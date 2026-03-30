#ifndef DEBUG_CONSOLE_H
#define DEBUG_CONSOLE_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stdint.h>

void debug_console_init(void);
void debug_console_write(const char *str);
void debug_console_write_line(const char *str);
bool debug_console_read_line(char *buffer, uint16_t buffer_size);

#ifdef __cplusplus
}
#endif

#endif /* DEBUG_CONSOLE_H */
