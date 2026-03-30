#include "uart_if.h"
#include <string.h>

static uart_if_t *s_irq_iface = NULL;

bool uart_if_init(uart_if_t *iface, UART_HandleTypeDef *huart)
{
    if ((iface == NULL) || (huart == NULL))
    {
        return false;
    }

    iface->huart = huart;
    iface->rx_byte = 0U;
    iface->rx_callback = NULL;

    return true;
}

bool uart_if_write(uart_if_t *iface, const uint8_t *data, uint16_t len, uint32_t timeout)
{
    if ((iface == NULL) || (iface->huart == NULL) || (data == NULL) || (len == 0U))
    {
        return false;
    }

    return (HAL_UART_Transmit(iface->huart, (uint8_t *)data, len, timeout) == HAL_OK);
}

bool uart_if_write_string(uart_if_t *iface, const char *str, uint32_t timeout)
{
    if ((iface == NULL) || (iface->huart == NULL) || (str == NULL))
    {
        return false;
    }

    return uart_if_write(iface, (const uint8_t *)str, (uint16_t)strlen(str), timeout);
}

bool uart_if_read_byte(uart_if_t *iface, uint8_t *byte)
{
    if ((iface == NULL) || (iface->huart == NULL) || (byte == NULL))
    {
        return false;
    }

    return (HAL_UART_Receive(iface->huart, byte, 1U, 0U) == HAL_OK);
}

void uart_if_set_rx_callback(uart_if_t *iface, uart_if_rx_callback_t callback)
{
    if (iface == NULL)
    {
        return;
    }

    iface->rx_callback = callback;
}

bool uart_if_start_rx_it(uart_if_t *iface)
{
    if ((iface == NULL) || (iface->huart == NULL))
    {
        return false;
    }

    s_irq_iface = iface;

    return (HAL_UART_Receive_IT(iface->huart, &iface->rx_byte, 1U) == HAL_OK);
}

void uart_if_handle_rx_cplt(UART_HandleTypeDef *huart)
{
    if ((s_irq_iface == NULL) || (huart == NULL))
    {
        return;
    }

    if (huart != s_irq_iface->huart)
    {
        return;
    }

    if (s_irq_iface->rx_callback != NULL)
    {
        s_irq_iface->rx_callback(s_irq_iface->rx_byte);
    }

    (void)HAL_UART_Receive_IT(s_irq_iface->huart, &s_irq_iface->rx_byte, 1U);
}
