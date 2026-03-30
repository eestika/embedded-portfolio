#include "debug_console.h"
#include "uart_if.h"
#include "usart.h"
#include <string.h>

#define DEBUG_CONSOLE_TIMEOUT_MS     100U
#define DEBUG_CONSOLE_RX_LINE_SIZE   128U

static uart_if_t s_debug_uart;
static char s_rx_line[DEBUG_CONSOLE_RX_LINE_SIZE];
static uint16_t s_rx_index = 0U;

void debug_console_init(void)
{
    (void)uart_if_init(&s_debug_uart, &huart3);
    s_rx_index = 0U;
    memset(s_rx_line, 0, sizeof(s_rx_line));
}

void debug_console_write(const char *str)
{
    (void)uart_if_write_string(&s_debug_uart, str, DEBUG_CONSOLE_TIMEOUT_MS);
}

void debug_console_write_line(const char *str)
{
    debug_console_write(str);
    debug_console_write("\r\n");
}

bool debug_console_read_line(char *buffer, uint16_t buffer_size)
{
    uint8_t ch = 0U;

    if ((buffer == NULL) || (buffer_size == 0U))
    {
        return false;
    }

    if (!uart_if_read_byte(&s_debug_uart, &ch))
    {
        return false;
    }

    if ((ch == '\r') || (ch == '\n'))
    {
        if (s_rx_index == 0U)
        {
            return false;
        }

        s_rx_line[s_rx_index] = '\0';

        strncpy(buffer, s_rx_line, buffer_size - 1U);
        buffer[buffer_size - 1U] = '\0';

        s_rx_index = 0U;
        memset(s_rx_line, 0, sizeof(s_rx_line));
        return true;
    }

    if (s_rx_index < (DEBUG_CONSOLE_RX_LINE_SIZE - 1U))
    {
        s_rx_line[s_rx_index++] = (char)ch;
    }
    else
    {
        s_rx_index = 0U;
        memset(s_rx_line, 0, sizeof(s_rx_line));
        debug_console_write_line("ERR: RX overflow");
    }

    return false;
}/*
 * debug_console.c
 *
 *  Created on: 28 mar 2026
 *      Author: danie
 */


