#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>
#include <sys/select.h>

#include "serial_port.h"

static speed_t serial_port_map_baudrate(int baudrate)
{
    switch (baudrate)
    {
        case 9600:   return B9600;
        case 19200:  return B19200;
        case 38400:  return B38400;
        case 57600:  return B57600;
        case 115200: return B115200;
        default:     return B115200;
    }
}

int serial_port_open(const char *device, int baudrate)
{
    int fd;
    struct termios tty;
    speed_t speed;

    if (device == NULL)
    {
        return -1;
    }

    fd = open(device, O_RDWR | O_NOCTTY | O_SYNC);
    if (fd < 0)
    {
        perror("open");
        return -1;
    }

    memset(&tty, 0, sizeof(tty));
    if (tcgetattr(fd, &tty) != 0)
    {
        perror("tcgetattr");
        close(fd);
        return -1;
    }

    speed = serial_port_map_baudrate(baudrate);

    cfsetospeed(&tty, speed);
    cfsetispeed(&tty, speed);

    tty.c_cflag = (tty.c_cflag & ~CSIZE) | CS8;
    tty.c_iflag &= ~(IGNBRK | IXON | IXOFF | IXANY);
    tty.c_lflag = 0;
    tty.c_oflag = 0;
    tty.c_cc[VMIN] = 0;
    tty.c_cc[VTIME] = 0;

    tty.c_cflag |= (CLOCAL | CREAD);
    tty.c_cflag &= ~(PARENB | PARODD);
    tty.c_cflag &= ~CSTOPB;
    #ifdef CRTSCTS
    tty.c_cflag &= ~CRTSCTS;
#endif

    if (tcsetattr(fd, TCSANOW, &tty) != 0)
    {
        perror("tcsetattr");
        close(fd);
        return -1;
    }

    tcflush(fd, TCIOFLUSH);

    return fd;
}

int serial_port_close(int fd)
{
    return close(fd);
}

ssize_t serial_port_write_all(int fd, const void *buf, size_t len)
{
    const uint8_t *ptr = (const uint8_t *)buf;
    size_t total = 0;
    ssize_t written;

    while (total < len)
    {
        written = write(fd, ptr + total, len - total);
        if (written < 0)
        {
            if (errno == EINTR)
            {
                continue;
            }

            perror("write");
            return -1;
        }

        total += (size_t)written;
    }

    return (ssize_t)total;
}

ssize_t serial_port_read_byte_timeout(int fd, uint8_t *byte, int timeout_ms)
{
    fd_set read_fds;
    struct timeval tv;
    int rc;

    if (byte == NULL)
    {
        return -1;
    }

    FD_ZERO(&read_fds);
    FD_SET(fd, &read_fds);

    tv.tv_sec = timeout_ms / 1000;
    tv.tv_usec = (timeout_ms % 1000) * 1000;

    rc = select(fd + 1, &read_fds, NULL, NULL, &tv);
    if (rc < 0)
    {
        perror("select");
        return -1;
    }

    if (rc == 0)
    {
        return 0;
    }

    return read(fd, byte, 1);
}