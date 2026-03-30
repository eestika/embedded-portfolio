#include <stdio.h>
#include <string.h>

#include "app.h"
#include "config.h"
#include "serial_port.h"
#include "link.h"
#include "protocol.h"

#define APP_INPUT_LINE_MAX 128

static void app_print_menu(void)
{
    printf("\n");
    printf("=== SRM Raspberry Master Test Menu ===\n");
    printf("1) PING\n");
    printf("2) STATUS\n");
    printf("3) LED ON\n");
    printf("4) LED OFF\n");
    printf("5) HELP\n");
    printf("6) CUSTOM COMMAND\n");
    printf("q) QUIT\n");
    printf("Select: ");
    fflush(stdout);
}

static const char *app_choice_to_command(char choice)
{
    switch (choice)
    {
        case '1': return protocol_cmd_ping();
        case '2': return protocol_cmd_status();
        case '3': return protocol_cmd_led_on();
        case '4': return protocol_cmd_led_off();
        case '5': return protocol_cmd_help();
        default:  return NULL;
    }
}

static void app_trim_newline(char *str)
{
    size_t len;

    if (str == NULL)
    {
        return;
    }

    len = strlen(str);

    while (len > 0U && (str[len - 1U] == '\n' || str[len - 1U] == '\r'))
    {
        str[len - 1U] = '\0';
        len--;
    }
}

static int app_send_and_receive(int fd, const char *cmd)
{
    char rx_line[SRM_RX_LINE_MAX];

    if ((cmd == NULL) || (*cmd == '\0'))
    {
        printf("ERROR: empty command\n");
        return -1;
    }

    printf("TX -> %s\n", cmd);

    if (link_send_line(fd, cmd) != 0)
    {
        printf("ERROR: failed to send command\n");
        return -1;
    }

    if (link_read_line(fd, rx_line, sizeof(rx_line), SRM_DEFAULT_TIMEOUT_MS) != 0)
    {
        printf("ERROR: timeout or invalid response\n");
        return -1;
    }

    printf("RX <- %s\n", rx_line);
    return 0;
}

int app_run(const char *device, int baudrate)
{
    int fd;
    char menu_input[32];
    char custom_cmd[APP_INPUT_LINE_MAX];
    char choice;
    const char *cmd;

    printf("SRM Raspberry Master CLI\n");
    printf("Device   : %s\n", device);
    printf("Baudrate : %d\n", baudrate);

    fd = serial_port_open(device, baudrate);
    if (fd < 0)
    {
        printf("ERROR: cannot open serial device\n");
        return -1;
    }

    while (1)
    {
        app_print_menu();

        if (fgets(menu_input, sizeof(menu_input), stdin) == NULL)
        {
            printf("\nERROR: input failed\n");
            break;
        }

        choice = menu_input[0];

        if ((choice == 'q') || (choice == 'Q'))
        {
            printf("Exiting...\n");
            break;
        }

        if (choice == '6')
        {
            printf("Enter custom command: ");
            fflush(stdout);

            if (fgets(custom_cmd, sizeof(custom_cmd), stdin) == NULL)
            {
                printf("ERROR: failed to read custom command\n");
                continue;
            }

            app_trim_newline(custom_cmd);

            if (custom_cmd[0] == '\0')
            {
                printf("ERROR: custom command is empty\n");
                continue;
            }

            (void)app_send_and_receive(fd, custom_cmd);
            continue;
        }

        cmd = app_choice_to_command(choice);
        if (cmd == NULL)
        {
            printf("Invalid selection\n");
            continue;
        }

        (void)app_send_and_receive(fd, cmd);
    }

    serial_port_close(fd);
    return 0;
}