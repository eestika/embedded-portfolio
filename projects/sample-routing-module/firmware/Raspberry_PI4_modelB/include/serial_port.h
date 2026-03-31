#ifndef SERIAL_PORT_H
#define SERIAL_PORT_H

#include <stddef.h>
#include <stdint.h>
#include <sys/types.h>

int serial_port_open(const char *device, int baudrate);
int serial_port_close(int fd);

ssize_t serial_port_write_all(int fd, const void *buf, size_t len);
ssize_t serial_port_read_byte_timeout(int fd, uint8_t *byte, int timeout_ms);

#endif /* SERIAL_PORT_H */