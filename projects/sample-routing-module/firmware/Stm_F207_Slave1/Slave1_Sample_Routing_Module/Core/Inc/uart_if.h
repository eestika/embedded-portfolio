#ifndef UART_IF_H
#define UART_IF_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stdint.h>
#include "usart.h"

typedef void (*uart_if_rx_callback_t)(uint8_t byte);

typedef struct
{
    UART_HandleTypeDef *huart;
    uint8_t rx_byte;
    uart_if_rx_callback_t rx_callback;
} uart_if_t;

bool uart_if_init(uart_if_t *iface, UART_HandleTypeDef *huart);
bool uart_if_write(uart_if_t *iface, const uint8_t *data, uint16_t len, uint32_t timeout);
bool uart_if_write_string(uart_if_t *iface, const char *str, uint32_t timeout);
bool uart_if_read_byte(uart_if_t *iface, uint8_t *byte);

void uart_if_set_rx_callback(uart_if_t *iface, uart_if_rx_callback_t callback);
bool uart_if_start_rx_it(uart_if_t *iface);
void uart_if_handle_rx_cplt(UART_HandleTypeDef *huart);

#ifdef __cplusplus
}
#endif

#endif /* UART_IF_H */
