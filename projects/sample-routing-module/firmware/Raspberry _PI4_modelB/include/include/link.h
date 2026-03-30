#ifndef LINK_H
#define LINK_H

#include <stddef.h>

int link_send_line(int fd, const char *line);
int link_read_line(int fd, char *buf, size_t buf_size, int timeout_ms);

#endif /* LINK_H */