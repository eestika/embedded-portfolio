#include <stdio.h>
#include <stdint.h>
#include <unistd.h>

#include "config.h"
#include "serial_port.h"
#include "srm_defs.h"
#include "srm_crc.h"

static void print_frame_hex(const uint8_t *frame, int size)
{
    int i;

    for (i = 0; i < size; i++)
    {
        printf("%02X ", frame[i]);
    }
    printf("\n");
}

int main(void)
{
    int fd;
    uint8_t byte;
    uint8_t frame[SRM_MAX_FRAME_SIZE];
    ssize_t rc;
    int count = 0;
    int total_len;
    uint8_t payload_len;
    uint16_t crc_rx;
    uint16_t crc_calc;

    printf("SRM Binary RX Test\n");
    printf("Device   : %s\n", SRM_DEFAULT_DEVICE);
    printf("Baudrate : %d\n", SRM_DEFAULT_BAUDRATE);

    fd = serial_port_open(SRM_DEFAULT_DEVICE, SRM_DEFAULT_BAUDRATE);
    if (fd < 0)
    {
        printf("ERROR: cannot open serial device\n");
        return -1;
    }

    printf("Waiting for SOF 0x%02X...\n", SRM_SOF);

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
            frame[0] = byte;
            count = 1;
            break;
        }
    }

    /* Read remaining fixed header bytes up to LEN */
    while (count < SRM_HEADER_SIZE)
    {
        rc = serial_port_read_byte_timeout(fd, &byte, 3000);
        if (rc < 0)
        {
            printf("ERROR: serial read failed at header byte %d\n", count);
            serial_port_close(fd);
            return -1;
        }

        if (rc == 0)
        {
            printf("ERROR: timeout waiting for header byte %d\n", count);
            serial_port_close(fd);
            return -1;
        }

        frame[count] = byte;
        count++;
    }

    payload_len = frame[7];
    if (payload_len > SRM_MAX_PAYLOAD_LEN)
    {
        printf("ERROR: invalid payload length %u\n", payload_len);
        serial_port_close(fd);
        return -1;
    }

    total_len = SRM_HEADER_SIZE + payload_len + SRM_CRC_SIZE;

    printf("SOF found. Header received.\n");
    printf("LEN = %u, total frame size = %d bytes\n", payload_len, total_len);

    while (count < total_len)
    {
        rc = serial_port_read_byte_timeout(fd, &byte, 3000);
        if (rc < 0)
        {
            printf("ERROR: serial read failed at byte %d\n", count);
            serial_port_close(fd);
            return -1;
        }

        if (rc == 0)
        {
            printf("ERROR: timeout waiting for byte %d\n", count);
            serial_port_close(fd);
            return -1;
        }

        frame[count] = byte;
        count++;
    }

    printf("Frame received:\n");
    print_frame_hex(frame, total_len);

    crc_rx = ((uint16_t)frame[total_len - 2] << 8) | (uint16_t)frame[total_len - 1];
    crc_calc = srm_crc16_ccitt_false(&frame[1], (size_t)(7 + payload_len));

    printf("\nDecoded fields:\n");
    printf("SOF   : 0x%02X\n", frame[0]);
    printf("VER   : 0x%02X\n", frame[1]);
    printf("FLAGS : 0x%02X\n", frame[2]);
    printf("SRC   : 0x%02X\n", frame[3]);
    printf("DST   : 0x%02X\n", frame[4]);
    printf("CMD   : 0x%02X\n", frame[5]);
    printf("SEQ   : 0x%02X\n", frame[6]);
    printf("LEN   : 0x%02X\n", frame[7]);

    if (payload_len > 0)
    {
        int i;
        printf("\nPayload:\n");
        for (i = 0; i < payload_len; i++)
        {
            printf("PAYLOAD[%d] : 0x%02X\n", i, frame[8 + i]);
        }
    }

    printf("\nCRC received : 0x%04X\n", crc_rx);
    printf("CRC computed : 0x%04X\n", crc_calc);

    if (crc_rx == crc_calc)
    {
        printf("CRC check    : OK\n");
    }
    else
    {
        printf("CRC check    : FAIL\n");
    }

    printf("\nDone.\n");

    serial_port_close(fd);
    return 0;
}