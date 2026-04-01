/* lcd.h */
#ifndef LCD_H
#define LCD_H

#include <stdbool.h>
#include <stdint.h>

#define LCD_ROWS    2U
#define LCD_COLS    16U

void lcd_init(void);
void lcd_task(void);

bool lcd_clear_display(void);
bool lcd_write_text(uint8_t row, uint8_t col, const char *text);

/*
 * Convenience helper:
 * writes a full line starting at column 0.
 * Text longer than 16 chars is truncated.
 */
bool lcd_write_line(uint8_t row, const char *text);

#endif /* LCD_H */
