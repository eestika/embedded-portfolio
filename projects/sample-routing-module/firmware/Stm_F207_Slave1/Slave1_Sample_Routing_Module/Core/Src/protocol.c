#include "protocol.h"
#include "debug_console.h"
#include "main.h"
#include "rs485_if.h"

#include <string.h>
#include <stdio.h>

static uint8_t s_led_state = 0U;

void protocol_init(void)
{
    s_led_state = 0U;

    HAL_GPIO_WritePin(LED_HEARTBEAT_GPIO_Port, LED_HEARTBEAT_Pin, GPIO_PIN_RESET);

    rs485_if_init();
}

static void protocol_send_response(const char *response)
{
    if (response == NULL)
    {
        return;
    }

    if (rs485_if_send_string(response) && rs485_if_send_string("\r\n"))
    {
        debug_console_write("U1 TX: ");
        debug_console_write_line(response);
    }
    else
    {
        debug_console_write_line("ERR: failed to send RS485 response");
    }
}

static void protocol_cmd_ping(void)
{
    protocol_send_response("PONG");
}

static void protocol_cmd_help(void)
{
    protocol_send_response("CMDS: PING, STATUS, LED ON, LED OFF, HELP");
}

static void protocol_cmd_led_on(void)
{
    s_led_state = 1U;
    HAL_GPIO_WritePin(LED_HEARTBEAT_GPIO_Port, LED_HEARTBEAT_Pin, GPIO_PIN_SET);

    protocol_send_response("OK LED ON");
}

static void protocol_cmd_led_off(void)
{
    s_led_state = 0U;
    HAL_GPIO_WritePin(LED_HEARTBEAT_GPIO_Port, LED_HEARTBEAT_Pin, GPIO_PIN_RESET);

    protocol_send_response("OK LED OFF");
}

static void protocol_cmd_status(void)
{
    char response[32];

    if (s_led_state != 0U)
    {
        snprintf(response, sizeof(response), "STATUS LED=ON");
    }
    else
    {
        snprintf(response, sizeof(response), "STATUS LED=OFF");
    }

    protocol_send_response(response);
}

void protocol_process_line(const char *line)
{
    if (line == NULL)
    {
        return;
    }

    if (strcmp(line, "PING") == 0)
    {
        protocol_cmd_ping();
    }
    else if (strcmp(line, "HELP") == 0)
    {
        protocol_cmd_help();
    }
    else if (strcmp(line, "LED ON") == 0)
    {
        protocol_cmd_led_on();
    }
    else if (strcmp(line, "LED OFF") == 0)
    {
        protocol_cmd_led_off();
    }
    else if (strcmp(line, "STATUS") == 0)
    {
        protocol_cmd_status();
    }
    else
    {
        protocol_send_response("ERR UNKNOWN");

        debug_console_write("ERR: UNKNOWN CMD -> ");
        debug_console_write_line(line);
    }
}
