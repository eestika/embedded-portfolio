#include "rs485_if.h"
#include "main.h"
#include "usart.h"
#include "uart_if.h"
#include "protocol.h"
#include "debug_console.h"

#include <string.h>

#define RS485_TX_TIMEOUT_MS        100U
#define RS485_RX_LINE_BUFFER_SIZE  128U

static uart_if_t s_rs485_uart;
static char s_rx_line[RS485_RX_LINE_BUFFER_SIZE];
static uint16_t s_rx_index = 0U;

static void rs485_set_tx_mode(void)
{
#if defined(RS485_DE_Pin) && defined(RS485_DE_GPIO_Port)
    HAL_GPIO_WritePin(RS485_DE_GPIO_Port, RS485_DE_Pin, GPIO_PIN_SET);
#endif
}

static void rs485_set_rx_mode(void)
{
#if defined(RS485_DE_Pin) && defined(RS485_DE_GPIO_Port)
    HAL_GPIO_WritePin(RS485_DE_GPIO_Port, RS485_DE_Pin, GPIO_PIN_RESET);
#endif
}

static void rs485_reset_rx_line(void)
{
    memset(s_rx_line, 0, sizeof(s_rx_line));
    s_rx_index = 0U;
}

static void rs485_on_rx_byte(uint8_t byte)
{
    if (byte == '\r')
    {
        return;
    }

    if (byte == '\n')
    {
        if (s_rx_index > 0U)
        {
            s_rx_line[s_rx_index] = '\0';

            debug_console_write("U1 RX: ");
            debug_console_write_line(s_rx_line);

            protocol_process_line(s_rx_line);
        }

        rs485_reset_rx_line();
        return;
    }

    if (s_rx_index < (RS485_RX_LINE_BUFFER_SIZE - 1U))
    {
        s_rx_line[s_rx_index++] = (char)byte;
    }
    else
    {
        debug_console_write_line("ERR: USART1 RX line overflow");
        rs485_reset_rx_line();
    }
}

bool rs485_if_is_de_configured(void)
{
#if defined(RS485_DE_Pin) && defined(RS485_DE_GPIO_Port)
    return true;
#else
    return false;
#endif
}

void rs485_if_init(void)
{
    (void)uart_if_init(&s_rs485_uart, &huart1);
    uart_if_set_rx_callback(&s_rs485_uart, rs485_on_rx_byte);

    rs485_reset_rx_line();
    rs485_set_rx_mode();

    if (uart_if_start_rx_it(&s_rs485_uart))
    {
        debug_console_write_line("USART1 RX interrupt started");
    }
    else
    {
        debug_console_write_line("ERR: USART1 RX interrupt start failed");
    }
}

bool rs485_if_send_bytes(const uint8_t *data, uint16_t len)
{
    if ((data == NULL) || (len == 0U))
    {
        return false;
    }

    rs485_set_tx_mode();

    if (!uart_if_write(&s_rs485_uart, data, len, RS485_TX_TIMEOUT_MS))
    {
        rs485_set_rx_mode();
        return false;
    }

    while (__HAL_UART_GET_FLAG(&huart1, UART_FLAG_TC) == RESET)
    {
        /* wait */
    }

    rs485_set_rx_mode();
    return true;
}

bool rs485_if_send_string(const char *str)
{
    if (str == NULL)
    {
        return false;
    }

    return rs485_if_send_bytes((const uint8_t *)str, (uint16_t)strlen(str));
}

bool rs485_if_read_byte(uint8_t *byte)
{
    if (byte == NULL)
    {
        return false;
    }

    rs485_set_rx_mode();
    return uart_if_read_byte(&s_rs485_uart, byte);
}
