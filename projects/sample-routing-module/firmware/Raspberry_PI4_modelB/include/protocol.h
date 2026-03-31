#ifndef PROTOCOL_H
#define PROTOCOL_H

const char *protocol_cmd_ping(void);
const char *protocol_cmd_status(void);
const char *protocol_cmd_led_on(void);
const char *protocol_cmd_led_off(void);
const char *protocol_cmd_help(void);

#endif /* PROTOCOL_H */