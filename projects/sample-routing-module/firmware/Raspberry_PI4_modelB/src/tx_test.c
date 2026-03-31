#include <stdio.h>
#include <stdint.h>
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

int main(void)
{
    int fd;
    uint8_t tx_frame[SRM_MAX_FRAME_SIZE];
    uint8_t rx_frame[SRM_MAX_FRAME_SIZE];
    size_t tx_len = 0U;
    ssize_t rc;
    uint8_t byte;
    int count = 0;
    int total_len;
    uint8_t payload_len;
    uint16_t crc_rx;
    uint16_t crc_calc;

    printf("SRM Binary TX/RX Test\n");
    printf("Device   : %s\n", SRM_DEFAULT_DEVICE);
    printf("Baudrate : %d\n", SRM_DEFAULT_BAUDRATE);

    if (srm_build_frame(tx_frame,
                        sizeof(tx_frame),
                        &tx_len,
                        0x01U,
                        0x00U,
                        0x10U,
                        0x01U,
                        SRM_CMD_PING_REQ,
                        0x01U,
                        NULL,
                        0U) != 0)
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

    printf("PING_REQ sent. Waiting for PING_RSP...\n");

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
        if (rc < 0)
        {
            printf("ERROR: read failed at header byte %d\n", count);
            serial_port_close(fd);
            return -1;
        }

        if (rc == 0)
        {
            printf("ERROR: timeout waiting for header byte %d\n", count);
            serial_port_close(fd);
            return -1;
        }

        rx_frame[count++] = byte;
    }

    payload_len = rx_frame[7];
    if (payload_len > SRM_MAX_PAYLOAD_LEN)
    {
        printf("ERROR: invalid payload length %u\n", payload_len);
        serial_port_close(fd);
        return -1;
    }

    total_len = SRM_HEADER_SIZE + payload_len + SRM_CRC_SIZE;

    while (count < total_len)
    {
        rc = serial_port_read_byte_timeout(fd, &byte, 3000);
        if (rc < 0)
        {
            printf("ERROR: read failed at byte %d\n", count);
            serial_port_close(fd);
            return -1;
        }

        if (rc == 0)
        {
            printf("ERROR: timeout waiting for byte %d\n", count);
            serial_port_close(fd);
            return -1;
        }

        rx_frame[count++] = byte;
    }

    printf("RX frame:\n");
    print_frame_hex(rx_frame, (size_t)total_len);

    crc_rx = (uint16_t)(((uint16_t)rx_frame[total_len - 2] << 8) |
                        (uint16_t)rx_frame[total_len - 1]);
    crc_calc = srm_crc16_ccitt_false(&rx_frame[1], (size_t)(7U + payload_len));

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

    if (crc_rx == crc_calc)
    {
        printf("CRC check: OK\n");
    }
    else
    {
        printf("CRC check: FAIL\n");
    }

    if (rx_frame[5] == SRM_CMD_PING_RSP)
    {
        printf("PING_RSP received correctly.\n");
    }
    else
    {
        printf("Unexpected CMD received.\n");
    }

    serial_port_close(fd);
    return 0;
}