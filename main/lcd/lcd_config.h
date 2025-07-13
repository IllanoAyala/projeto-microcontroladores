#ifndef LCD_CONFIG_H
#define LCD_CONFIG_H

#include "driver/gpio.h"

void lcd_init(void);
void lcd_clear(void);
void lcd_set_cursor(uint8_t col, uint8_t row);
void lcd_write_string_and_temp(const char *str, float value, uint8_t current_unit);
void lcd_write_only_string(const char *str);
void lcd_scroll_text(const char *str, uint8_t row, uint32_t delay_ms);

#endif
