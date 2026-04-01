/* lcd.c */
#include "lcd.h"

#include "i2c_if.h"
#include "i2c.h"
#include "main.h"

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>

#define LCD_I2C_TIMEOUT_MS         100U

/* PCF8574 backpack address */
#define LCD_I2C_ADDR_7BIT          0x27U
#define LCD_I2C_ADDR_HAL           (LCD_I2C_ADDR_7BIT << 1)

/* PCF8574 -> LCD pin mapping */
#define LCD_PIN_RS                 0x01U
#define LCD_PIN_RW                 0x02U
#define LCD_PIN_EN                 0x04U
#define LCD_PIN_BL                 0x08U

#define LCD_BACKLIGHT_ON           LCD_PIN_BL

/* HD44780 commands */
#define LCD_CMD_CLEAR_DISPLAY      0x01U
#define LCD_CMD_RETURN_HOME        0x02U
#define LCD_CMD_ENTRY_MODE_SET     0x04U
#define LCD_CMD_DISPLAY_CONTROL    0x08U
#define LCD_CMD_FUNCTION_SET       0x20U
#define LCD_CMD_SET_DDRAM_ADDR     0x80U

/* Command flags */
#define LCD_ENTRY_INCREMENT        0x02U
#define LCD_ENTRY_SHIFT_OFF        0x00U

#define LCD_DISPLAY_ON             0x04U
#define LCD_CURSOR_OFF             0x00U
#define LCD_BLINK_OFF              0x00U

#define LCD_4BIT_MODE              0x00U
#define LCD_2LINE_MODE             0x08U
#define LCD_5X8_DOTS               0x00U

typedef struct
{
    bool initialized;
    i2c_if_t i2c;
    uint8_t backlight;
} lcd_ctx_t;

static lcd_ctx_t s_lcd;

/* Private helper prototypes */
static void lcd_delay_ms(uint32_t delay_ms);
static bool lcd_write_expander(uint8_t data);
static bool lcd_pulse_enable(uint8_t data);
static bool lcd_write4bits(uint8_t nibble_with_ctrl);
static bool lcd_send_byte(uint8_t value, uint8_t rs);
static bool lcd_send_command(uint8_t cmd);
static bool lcd_send_data_byte(uint8_t data);
static bool lcd_set_cursor(uint8_t row, uint8_t col);
static bool lcd_write_raw_text(const char *text);
static uint8_t lcd_row_to_ddram_base(uint8_t row);

void lcd_init(void)
{
    bool ok = true;

    memset(&s_lcd, 0, sizeof(s_lcd));

    s_lcd.backlight = LCD_BACKLIGHT_ON;

    ok = i2c_if_init(&s_lcd.i2c, &hi2c1);

    if (!ok)
    {
        s_lcd.initialized = false;
        return;
    }

    HAL_Delay(100U);

    ok &= lcd_write_expander(s_lcd.backlight);
    HAL_Delay(20U);

    /*
     * HD44780 reset sequence in 4-bit mode:
     * 0x3, 0x3, 0x3, 0x2
     */
    ok &= lcd_write4bits(0x30U);
    HAL_Delay(10U);

    ok &= lcd_write4bits(0x30U);
    HAL_Delay(10U);

    ok &= lcd_write4bits(0x30U);
    HAL_Delay(10U);

    ok &= lcd_write4bits(0x20U);
    HAL_Delay(10U);

    ok &= lcd_send_command((uint8_t)(LCD_CMD_FUNCTION_SET |
                                     LCD_4BIT_MODE |
                                     LCD_2LINE_MODE |
                                     LCD_5X8_DOTS));

    ok &= lcd_send_command(LCD_CMD_DISPLAY_CONTROL);

    ok &= lcd_send_command(LCD_CMD_CLEAR_DISPLAY);

    ok &= lcd_send_command((uint8_t)(LCD_CMD_ENTRY_MODE_SET |
                                     LCD_ENTRY_INCREMENT |
                                     LCD_ENTRY_SHIFT_OFF));

    ok &= lcd_send_command((uint8_t)(LCD_CMD_DISPLAY_CONTROL |
                                     LCD_DISPLAY_ON |
                                     LCD_CURSOR_OFF |
                                     LCD_BLINK_OFF));

    s_lcd.initialized = ok;
}

void lcd_task(void)
{
    /* No periodic background processing required */
}

bool lcd_clear_display(void)
{
    if (!s_lcd.initialized)
    {
        return false;
    }

    if (!lcd_send_command(LCD_CMD_CLEAR_DISPLAY))
    {
        return false;
    }

    HAL_Delay(5U);
    return true;
}

bool lcd_write_text(uint8_t row, uint8_t col, const char *text)
{
    size_t text_len;
    size_t writable_len;
    char local_buf[LCD_COLS + 1U];

    if (!s_lcd.initialized)
    {
        return false;
    }

    if (text == NULL)
    {
        return false;
    }

    if (row >= LCD_ROWS)
    {
        return false;
    }

    if (col >= LCD_COLS)
    {
        return false;
    }

    text_len = strlen(text);

    if (text_len == 0U)
    {
        return true;
    }

    writable_len = text_len;

    if (((size_t)col + writable_len) > LCD_COLS)
    {
        writable_len = (size_t)LCD_COLS - (size_t)col;
    }

    memset(local_buf, ' ', sizeof(local_buf));
    memcpy(local_buf, text, writable_len);
    local_buf[LCD_COLS] = '\0';

    if (!lcd_set_cursor(row, col))
    {
        return false;
    }

    return lcd_write_raw_text(local_buf);
}

bool lcd_write_line(uint8_t row, const char *text)
{
    return lcd_write_text(row, 0U, text);
}

/* ============================================================================
 * Private helpers
 * ========================================================================== */

static void lcd_delay_ms(uint32_t delay_ms)
{
    HAL_Delay(delay_ms);
}

static bool lcd_write_expander(uint8_t data)
{
    return i2c_if_write(&s_lcd.i2c,
                        LCD_I2C_ADDR_HAL,
                        &data,
                        1U,
                        LCD_I2C_TIMEOUT_MS);
}

static bool lcd_pulse_enable(uint8_t data)
{
    if (!lcd_write_expander((uint8_t)(data | LCD_PIN_EN)))
    {
        return false;
    }

    lcd_delay_ms(1U);

    if (!lcd_write_expander((uint8_t)(data & (uint8_t)(~LCD_PIN_EN))))
    {
        return false;
    }

    lcd_delay_ms(1U);

    return true;
}

static bool lcd_write4bits(uint8_t nibble_with_ctrl)
{
    uint8_t data;

    data = (uint8_t)(nibble_with_ctrl | s_lcd.backlight);

    if (!lcd_write_expander(data))
    {
        return false;
    }

    return lcd_pulse_enable(data);
}

static bool lcd_send_byte(uint8_t value, uint8_t rs)
{
    uint8_t high_nibble;
    uint8_t low_nibble;
    uint8_t ctrl;

    ctrl = (rs != 0U) ? LCD_PIN_RS : 0U;

    high_nibble = (uint8_t)(value & 0xF0U);
    low_nibble = (uint8_t)((value << 4) & 0xF0U);

    if (!lcd_write4bits((uint8_t)(high_nibble | ctrl)))
    {
        return false;
    }

    if (!lcd_write4bits((uint8_t)(low_nibble | ctrl)))
    {
        return false;
    }

    return true;
}

static bool lcd_send_command(uint8_t cmd)
{
    if (!lcd_send_byte(cmd, 0U))
    {
        return false;
    }

    if ((cmd == LCD_CMD_CLEAR_DISPLAY) ||
        (cmd == LCD_CMD_RETURN_HOME))
    {
        lcd_delay_ms(5U);
    }
    else
    {
        lcd_delay_ms(2U);
    }

    return true;
}

static bool lcd_send_data_byte(uint8_t data)
{
    if (!lcd_send_byte(data, 1U))
    {
        return false;
    }

    lcd_delay_ms(1U);
    return true;
}

static bool lcd_set_cursor(uint8_t row, uint8_t col)
{
    uint8_t ddram_addr;

    ddram_addr = (uint8_t)(lcd_row_to_ddram_base(row) + col);

    return lcd_send_command((uint8_t)(LCD_CMD_SET_DDRAM_ADDR | ddram_addr));
}

static bool lcd_write_raw_text(const char *text)
{
    size_t len;
    size_t i;

    if (text == NULL)
    {
        return false;
    }

    len = strlen(text);

    if (len > LCD_COLS)
    {
        len = LCD_COLS;
    }

    for (i = 0U; i < len; i++)
    {
        if (!lcd_send_data_byte((uint8_t)text[i]))
        {
            return false;
        }
    }

    return true;
}

static uint8_t lcd_row_to_ddram_base(uint8_t row)
{
    if (row == 0U)
    {
        return 0x00U;
    }

    return 0x40U;
}
