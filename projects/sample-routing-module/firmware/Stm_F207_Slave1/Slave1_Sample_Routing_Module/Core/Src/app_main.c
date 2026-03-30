#include "app_main.h"
#include "main.h"
#include "debug_console.h"
#include "protocol.h"
#include "lcd.h"
#include "eeprom.h"
#include "rs485_if.h"

#include <string.h>

#define APP_RX_BUFFER_SIZE              128U
#define HEARTBEAT_PERIOD_MS             500U
#define USART1_TEST_PERIOD_MS           500U
#define USART1_CONTINUOUS_TEST_ENABLE   0U

static uint32_t s_last_heartbeat_ms = 0U;
static uint32_t s_last_u1_test_ms = 0U;
static char s_line_buffer[APP_RX_BUFFER_SIZE];

static void app_heartbeat_task(void)
{
    uint32_t now = HAL_GetTick();

    if ((now - s_last_heartbeat_ms) >= HEARTBEAT_PERIOD_MS)
    {
        s_last_heartbeat_ms = now;
        HAL_GPIO_TogglePin(LD3_GPIO_Port, LD3_Pin);
    }
}

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

#if USART1_CONTINUOUS_TEST_ENABLE
static void app_usart1_continuous_test_task(void)
{
    uint32_t now = HAL_GetTick();

    if ((now - s_last_u1_test_ms) >= USART1_TEST_PERIOD_MS)
    {
        s_last_u1_test_ms = now;
        (void)rs485_if_send_string("U1_TEST_1234\r\n");
    }
}
#endif

void app_main_init(void)
{
    debug_console_init();
    protocol_init();
    lcd_init();
    eeprom_init();

    HAL_GPIO_WritePin(LED_HEARTBEAT_GPIO_Port, LED_HEARTBEAT_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(LD3_GPIO_Port, LD3_Pin, GPIO_PIN_RESET);

    debug_console_write_line("");
    debug_console_write_line("=== Slave1 Sample Routing Module ===");
    debug_console_write_line("Debug UART : USART3");
    debug_console_write_line("App UART   : USART1");
    debug_console_write_line("I2C Bus    : I2C1");

#if USART1_CONTINUOUS_TEST_ENABLE
    debug_console_write_line("USART1 continuous test ENABLED");
#else
    debug_console_write_line("USART1 continuous test DISABLED");
#endif

    debug_console_write_line("");
}

void app_main_run(void)
{
    app_heartbeat_task();
    app_console_task();

#if USART1_CONTINUOUS_TEST_ENABLE
    app_usart1_continuous_test_task();
#endif

    lcd_task();
}
