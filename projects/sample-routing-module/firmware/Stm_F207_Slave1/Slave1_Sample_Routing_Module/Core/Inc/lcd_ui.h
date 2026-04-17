#ifndef LCD_UI_H
#define LCD_UI_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stdint.h>

void lcd_ui_init(void);
void lcd_ui_task(void);

bool lcd_ui_show_ready(void);
bool lcd_ui_show_temporary_message(const char *text, uint32_t timeout_ms);
void lcd_ui_enter_manual_mode(void);

#ifdef __cplusplus
}
#endif

#endif /* LCD_UI_H */
