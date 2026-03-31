#include <stdio.h>
#include <string.h>
#include <stdint.h>

#include "link.h"
#include "serial_port.h"

int link_send_line(int fd, const char *line)
{
    char tx_buffer[256];
    int written_len;

    if (line == NULL)
    {
        return -1;
    }

    written_len = snprintf(tx_buffer, sizeof(tx_buffer), "%s\n", line);
    if (written_len <= 0 || written_len >= (int)sizeof(tx_buffer))
    {
        return -1;
    }

    if (serial_port_write_all(fd, tx_buffer, (size_t)written_len) != written_len)
    {
        return -1;
    }

    return 0;
}

int link_read_line(int fd, char *buf, size_t buf_size, int timeout_ms)
{
    size_t index = 0;
    uint8_t byte = 0;
    ssize_t rc;

    if (buf == NULL || buf_size == 0U)
    {
        return -1;
    }

    while (index < (buf_size - 1U))
    {
        rc = serial_port_read_byte_timeout(fd, &byte, timeout_ms);
        if (rc <= 0)
        {
            return -1;
        }

        if (byte == '\r')
        {
            continue;
        }

        if (byte == '\n')
        {
            buf[index] = '\0';
            return 0;
        }

        buf[index++] = (char)byte;
    }

    buf[buf_size - 1U] = '\0';
    return -1;
}