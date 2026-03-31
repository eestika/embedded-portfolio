#include "protocol.h"

const char *protocol_cmd_ping(void)
{
    return "PING";
}

const char *protocol_cmd_status(void)
{
    return "STATUS";
}

const char *protocol_cmd_led_on(void)
{
    return "LED ON";
}

const char *protocol_cmd_led_off(void)
{
    return "LED OFF";
}

const char *protocol_cmd_help(void)
{
    return "HELP";
}