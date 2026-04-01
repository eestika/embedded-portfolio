#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <sys/types.h>

#include "config.h"
#include "serial_port.h"
#include "srm_defs.h"
#include "srm_crc.h"
#include "srm_frame_builder.h"

static void print_frame_hex(const uint8_t *buf, size_t len)
{
    size_t i;

    for (i = 0; i < len; i++)
    {
        printf("%02X ", buf[i]);
    }
    printf("\n");
}

static uint32_t decode_u32_be(const uint8_t *data)
{
    return ((uint32_t)data[0] << 24) |
           ((uint32_t)data[1] << 16) |
           ((uint32_t)data[2] << 8)  |
           ((uint32_t)data[3]);
}

static int send_and_receive_payload(uint8_t cmd_req,
                                    uint8_t cmd_rsp_expected,
                                    uint8_t seq,
                                    const uint8_t *payload,
                                    uint8_t payload_len)
{
    int fd;
    uint8_t tx_frame[SRM_MAX_FRAME_SIZE];
    uint8_t rx_frame[SRM_MAX_FRAME_SIZE];
    size_t tx_len = 0U;
    ssize_t rc;
    uint8_t byte;
    int count = 0;
    int total_len;
    uint8_t rx_payload_len;
    uint16_t crc_rx;
    uint16_t crc_calc;

    if (srm_build_frame(tx_frame,
                        sizeof(tx_frame),
                        &tx_len,
                        SRM_VERSION,
                        0x00U,
                        SRM_NODE_ID_MASTER,
                        SRM_NODE_ID_SLAVE1,
                        cmd_req,
                        seq,
                        payload,
                        payload_len) != 0)
    {
        printf("ERROR: build frame failed\n");
        return -1;
    }

    printf("TX frame:\n");
    print_frame_hex(tx_frame, tx_len);

    fd = serial_port_open(SRM_DEFAULT_DEVICE, SRM_DEFAULT_BAUDRATE);
    if (fd < 0)
    {
        printf("ERROR: cannot open serial device\n");
        return -1;
    }

    if (serial_port_write_all(fd, tx_frame, tx_len) != (ssize_t)tx_len)
    {
        printf("ERROR: serial write failed\n");
        serial_port_close(fd);
        return -1;
    }

    printf("Request sent. Waiting for response...\n");

    while (1)
    {
        rc = serial_port_read_byte_timeout(fd, &byte, 3000);
        if (rc < 0)
        {
            printf("ERROR: serial read failed while waiting for SOF\n");
            serial_port_close(fd);
            return -1;
        }

        if (rc == 0)
        {
            printf("ERROR: timeout waiting for SOF\n");
            serial_port_close(fd);
            return -1;
        }

        if (byte == SRM_SOF)
        {
            rx_frame[0] = byte;
            count = 1;
            break;
        }
    }

    while (count < SRM_HEADER_SIZE)
    {
        rc = serial_port_read_byte_timeout(fd, &byte, 3000);
        if (rc <= 0)
        {
            printf("ERROR: failed while reading header\n");
            serial_port_close(fd);
            return -1;
        }

        rx_frame[count++] = byte;
    }

    rx_payload_len = rx_frame[7];
    if (rx_payload_len > SRM_MAX_PAYLOAD_LEN)
    {
        printf("ERROR: invalid payload length %u\n", rx_payload_len);
        serial_port_close(fd);
        return -1;
    }

    total_len = SRM_HEADER_SIZE + rx_payload_len + SRM_CRC_SIZE;

    while (count < total_len)
    {
        rc = serial_port_read_byte_timeout(fd, &byte, 3000);
        if (rc <= 0)
        {
            printf("ERROR: failed while reading frame body\n");
            serial_port_close(fd);
            return -1;
        }

        rx_frame[count++] = byte;
    }

    printf("RX frame:\n");
    print_frame_hex(rx_frame, (size_t)total_len);

    crc_rx = (uint16_t)(((uint16_t)rx_frame[total_len - 2] << 8) |
                        (uint16_t)rx_frame[total_len - 1]);
    crc_calc = srm_crc16_ccitt_false(&rx_frame[1], (size_t)(7U + rx_payload_len));

    printf("Decoded RX:\n");
    printf("VER   : 0x%02X\n", rx_frame[1]);
    printf("FLAGS : 0x%02X\n", rx_frame[2]);
    printf("SRC   : 0x%02X\n", rx_frame[3]);
    printf("DST   : 0x%02X\n", rx_frame[4]);
    printf("CMD   : 0x%02X\n", rx_frame[5]);
    printf("SEQ   : 0x%02X\n", rx_frame[6]);
    printf("LEN   : 0x%02X\n", rx_frame[7]);
    printf("CRC RX   : 0x%04X\n", crc_rx);
    printf("CRC CALC : 0x%04X\n", crc_calc);

    if (crc_rx != crc_calc)
    {
        printf("CRC check: FAIL\n");
        serial_port_close(fd);
        return -1;
    }

    printf("CRC check: OK\n");

    if (rx_frame[5] != cmd_rsp_expected)
    {
        printf("Unexpected CMD received\n");
        serial_port_close(fd);
        return -1;
    }

    printf("Expected response received.\n");

    if ((rx_frame[5] == SRM_CMD_STATUS_RSP) && (rx_payload_len == 5U))
    {
        uint8_t led_state = rx_frame[8];
        uint32_t uptime_ms = decode_u32_be(&rx_frame[9]);

        printf("STATUS payload:\n");
        printf("LED state  : %s\n", (led_state == SRM_LED_STATE_ON) ? "ON" : "OFF");
        printf("Uptime ms  : %lu\n", (unsigned long)uptime_ms);
    }

    serial_port_close(fd);
    return 0;
}

static int send_and_receive(uint8_t cmd_req, uint8_t cmd_rsp_expected, uint8_t seq)
{
    return send_and_receive_payload(cmd_req, cmd_rsp_expected, seq, NULL, 0U);
}

static int send_lcd_write(uint8_t row, uint8_t col, const char *text, uint8_t seq)
{
    uint8_t payload[3U + SRM_LCD_MAX_TEXT_LEN];
    uint8_t text_len;

    if (text == NULL)
    {
        return -1;
    }

    text_len = (uint8_t)strlen(text);
    if (text_len > SRM_LCD_MAX_TEXT_LEN)
    {
        text_len = SRM_LCD_MAX_TEXT_LEN;
    }

    payload[0] = row;
    payload[1] = col;
    payload[2] = text_len;
    memcpy(&payload[3], text, text_len);

    return send_and_receive_payload(SRM_CMD_LCD_WRITE_REQ,
                                    SRM_CMD_LCD_WRITE_RSP,
                                    seq,
                                    payload,
                                    (uint8_t)(3U + text_len));
}

int main(void)
{
    char choice[16];
    uint8_t seq = 1U;

    printf("=== SRM Raspberry Menu Test ===\n");
    printf("Device   : %s\n", SRM_DEFAULT_DEVICE);
    printf("Baudrate : %d\n\n", SRM_DEFAULT_BAUDRATE);

    while (1)
    {
        printf("\n");
        printf("1) PING\n");
        printf("2) LED ON\n");
        printf("3) LED OFF\n");
        printf("4) STATUS\n");
        printf("5) LCD CLEAR\n");
        printf("6) LCD WRITE LINE 1\n");
        printf("7) LCD WRITE LINE 2\n");
        printf("q) QUIT\n");
        printf("Select: ");

        if (fgets(choice, sizeof(choice), stdin) == NULL)
        {
            continue;
        }

        if (choice[0] == '1')
        {
            (void)send_and_receive(SRM_CMD_PING_REQ, SRM_CMD_PING_RSP, seq++);
        }
        else if (choice[0] == '2')
        {
            (void)send_and_receive(SRM_CMD_LED_ON_REQ, SRM_CMD_LED_ON_RSP, seq++);
        }
        else if (choice[0] == '3')
        {
            (void)send_and_receive(SRM_CMD_LED_OFF_REQ, SRM_CMD_LED_OFF_RSP, seq++);
        }
        else if (choice[0] == '4')
        {
            (void)send_and_receive(SRM_CMD_STATUS_REQ, SRM_CMD_STATUS_RSP, seq++);
        }
        else if (choice[0] == '5')
        {
            (void)send_and_receive(SRM_CMD_LCD_CLEAR_REQ, SRM_CMD_LCD_CLEAR_RSP, seq++);
        }
        else if (choice[0] == '6')
        {
            (void)send_lcd_write(0U, 0U, "HELLO L1", seq++);
        }
        else if (choice[0] == '7')
        {
            (void)send_lcd_write(1U, 0U, "HELLO L2", seq++);
        }
        else if ((choice[0] == 'q') || (choice[0] == 'Q'))
        {
            printf("Exit.\n");
            break;
        }
        else
        {
            printf("Invalid selection.\n");
        }
    }

    return 0;
}