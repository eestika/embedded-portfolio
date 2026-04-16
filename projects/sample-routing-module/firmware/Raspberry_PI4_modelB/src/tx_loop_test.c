#define _DEFAULT_SOURCE

#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <sys/types.h>

#include "config.h"
#include "serial_port.h"

static void print_bytes(const uint8_t *data, size_t len)
{
    size_t i;

    for (i = 0; i < len; i++)
    {
        printf("%02X ", data[i]);
    }
    printf("\n");
}

int main(void)
{
    int fd;
    uint8_t test_data[] = {0x55, 0xAA, 0x10, 0x20, 0x30, 0x40, 0x0D, 0x0A};

    printf("SRM TX LOOP TEST\n");
    printf("Device   : %s\n", SRM_DEFAULT_DEVICE);
    printf("Baudrate : %d\n", SRM_DEFAULT_BAUDRATE);

    fd = serial_port_open(SRM_DEFAULT_DEVICE, SRM_DEFAULT_BAUDRATE);
    if (fd < 0)
    {
        printf("ERROR: cannot open serial device\n");
        return -1;
    }

    while (1)
    {
        size_t i;

        printf("Sending: ");
        print_bytes(test_data, sizeof(test_data));

        for (i = 0; i < sizeof(test_data); i++)
        {
            if (serial_port_write_all(fd, &test_data[i], 1) != 1)
            {
                printf("ERROR: serial write failed\n");
                serial_port_close(fd);
                return -1;
            }

            usleep(10000); /* 10 ms between bytes */
        }

        sleep(1);
    }

    serial_port_close(fd);
    return 0;
}