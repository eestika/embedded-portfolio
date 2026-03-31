#include "app_main.h"
#include "main.h"
#include "debug_console.h"
#include "protocol.h"
#include "lcd.h"
#include "eeprom.h"
#include "rs485_if.h"

#include <string.h>

#define APP_RX_BUFFER_SIZE    128U

static char s_line_buffer[APP_RX_BUFFER_SIZE];

static void app_console_task(void)
{
    if (debug_console_read_line(s_line_buffer, sizeof(s_line_buffer)))
    {
        debug_console_write("RX: ");
        debug_console_write_line(s_line_buffer);

        protocol_process_line(s_line_buffer);

        memset(s_line_buffer, 0, sizeof(s_line_buffer));
    }
}

void app_main_init(void)
{
    debug_console_init();
    protocol_init();
    lcd_init();
    eeprom_init();

    /* All LEDs start OFF */
    HAL_GPIO_WritePin(LED_HEARTBEAT_GPIO_Port, LED_HEARTBEAT_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(LD3_GPIO_Port, LD3_Pin, GPIO_PIN_RESET);

    debug_console_write_line("");
    debug_console_write_line("=== Slave1 Sample Routing Module ===");
    debug_console_write_line("Debug UART : USART3");
    debug_console_write_line("App UART   : USART1");
    debug_console_write_line("I2C Bus    : I2C1");
    debug_console_write_line("Heartbeat task DISABLED");
    debug_console_write_line("LD3 is controlled only by SRM commands");
    debug_console_write_line("");
}

void app_main_run(void)
{
    app_console_task();

    /* Process SRM RX outside ISR/callback */
    rs485_if_process_rx();

    lcd_task();
}
