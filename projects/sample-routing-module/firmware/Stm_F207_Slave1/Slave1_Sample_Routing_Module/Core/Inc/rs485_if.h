#ifndef RS485_IF_H
#define RS485_IF_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stdint.h>

void rs485_if_init(void);
bool rs485_if_send_bytes(const uint8_t *data, uint16_t len);
bool rs485_if_send_string(const char *str);
bool rs485_if_read_byte(uint8_t *byte);
bool rs485_if_is_de_configured(void);

#ifdef __cplusplus
}
#endif

#endif /* RS485_IF_H */
