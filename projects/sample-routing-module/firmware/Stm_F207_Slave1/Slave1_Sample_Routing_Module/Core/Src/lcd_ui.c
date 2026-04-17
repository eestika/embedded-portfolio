#include "lcd_ui.h"

#include "lcd.h"
#include "main.h"

#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#define LCD_UI_MAX_TEXT_LEN             16U
#define LCD_UI_DEFAULT_TEMP_TIMEOUT_MS  1200U

typedef enum
{
    LCD_UI_STATE_READY = 0,
    LCD_UI_STATE_TEMPORARY,
    LCD_UI_STATE_MANUAL
} lcd_ui_state_t;

typedef struct
{
    lcd_ui_state_t state;
    uint32_t deadline_ms;
    char current_text[LCD_UI_MAX_TEXT_LEN + 1U];
} lcd_ui_ctx_t;

static lcd_ui_ctx_t s_lcd_ui;

static bool lcd_ui_render_single_line(const char *text);
static bool lcd_ui_deadline_expired(uint32_t now, uint32_t deadline);

void lcd_ui_init(void)
{
    memset(&s_lcd_ui, 0, sizeof(s_lcd_ui));
    (void)lcd_ui_show_ready();
}

void lcd_ui_task(void)
{
    uint32_t now_ms;

    if (s_lcd_ui.state != LCD_UI_STATE_TEMPORARY)
    {
        return;
    }

    now_ms = HAL_GetTick();

    if (lcd_ui_deadline_expired(now_ms, s_lcd_ui.deadline_ms))
    {
        (void)lcd_ui_show_ready();
    }
}

bool lcd_ui_show_ready(void)
{
    if (!lcd_ui_render_single_line("READY"))
    {
        return false;
    }

    s_lcd_ui.state = LCD_UI_STATE_READY;
    s_lcd_ui.deadline_ms = 0U;
    memset(s_lcd_ui.current_text, 0, sizeof(s_lcd_ui.current_text));

    return true;
}

bool lcd_ui_show_temporary_message(const char *text, uint32_t timeout_ms)
{
    size_t len;

    if (text == NULL)
    {
        return false;
    }

    if (timeout_ms == 0U)
    {
        timeout_ms = LCD_UI_DEFAULT_TEMP_TIMEOUT_MS;
    }

    if (!lcd_ui_render_single_line(text))
    {
        return false;
    }

    memset(s_lcd_ui.current_text, 0, sizeof(s_lcd_ui.current_text));
    len = strlen(text);
    if (len > LCD_UI_MAX_TEXT_LEN)
    {
        len = LCD_UI_MAX_TEXT_LEN;
    }
    memcpy(s_lcd_ui.current_text, text, len);
    s_lcd_ui.current_text[len] = '\0';

    s_lcd_ui.state = LCD_UI_STATE_TEMPORARY;
    s_lcd_ui.deadline_ms = HAL_GetTick() + timeout_ms;

    return true;
}

void lcd_ui_enter_manual_mode(void)
{
    s_lcd_ui.state = LCD_UI_STATE_MANUAL;
    s_lcd_ui.deadline_ms = 0U;
    memset(s_lcd_ui.current_text, 0, sizeof(s_lcd_ui.current_text));
}

static bool lcd_ui_render_single_line(const char *text)
{
    if (!lcd_clear_display())
    {
        return false;
    }

    if (!lcd_write_line(0U, text))
    {
        return false;
    }

    return true;
}

static bool lcd_ui_deadline_expired(uint32_t now, uint32_t deadline)
{
    return ((int32_t)(now - deadline) >= 0);
}
