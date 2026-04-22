#include "app_main.h"
#include "main.h"
#include "debug_console.h"
#include "lcd.h"
#include "lcd_ui.h"
#include "eeprom.h"
#include "rs485_if.h"
#include "stepper.h"

void app_main_init(void)
{
    debug_console_init();
    lcd_init();
    lcd_ui_init();
    eeprom_init();
    rs485_if_init();

    if (!stepper_init())
    {
        Error_Handler();
    }

    /* All LEDs start OFF */
    HAL_GPIO_WritePin(LED_HEARTBEAT_GPIO_Port, LED_HEARTBEAT_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(LD3_GPIO_Port, LD3_Pin, GPIO_PIN_RESET);

    debug_console_write_line("");
    debug_console_write_line("=== Slave1 Sample Routing Module ===");
    debug_console_write_line("Debug UART : USART3");
    debug_console_write_line("App UART   : USART1");
    debug_console_write_line("I2C Bus    : I2C1");
    debug_console_write_line("ASCII protocol REMOVED");
    debug_console_write_line("SRM binary pipeline ACTIVE");
    debug_console_write_line("Heartbeat task DISABLED");
    debug_console_write_line("LD3 is controlled only by SRM commands");
    debug_console_write_line("LCD UI READY state ACTIVE");
    debug_console_write_line("Stepper    : PD4->IN1 PD5->IN2 PD6->IN3 PD7->IN4");
    debug_console_write_line("");
}

void app_main_run(void)
{
    lcd_task();
    lcd_ui_task();
    rs485_if_process_rx();
}
