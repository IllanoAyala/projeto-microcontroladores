#include "lcd_config.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <string.h>
#include <stdio.h>
#include "config.h"
#include "convert_temperature.h"

#define LCD_WRITE_PIN(pin, level) gpio_set_level(pin, level)

static void lcd_pulse_enable(void);
static void lcd_write_nibble(uint8_t nibble);
static void lcd_send(uint8_t data, bool is_data);
static void lcd_write_command(uint8_t cmd);
static void lcd_write_data(uint8_t data);

void lcd_init(void)
{
    gpio_config_t conf = {
        .mode = GPIO_MODE_OUTPUT,
        .pin_bit_mask =
            (1ULL << LCD_RS) |
            (1ULL << LCD_EN) |
            (1ULL << LCD_D4) |
            (1ULL << LCD_D5) |
            (1ULL << LCD_D6) |
            (1ULL << LCD_D7),
    };
    gpio_config(&conf);

    vTaskDelay(pdMS_TO_TICKS(100));  // tempo de inicialização

    // Sequência de inicialização padrão para 4 bits
    lcd_write_nibble(0x03);
    vTaskDelay(pdMS_TO_TICKS(5));
    lcd_write_nibble(0x03);
    vTaskDelay(pdMS_TO_TICKS(5));
    lcd_write_nibble(0x03);
    vTaskDelay(pdMS_TO_TICKS(1));
    lcd_write_nibble(0x02); // Modo 4 bits

    lcd_write_command(0x28); // 4 bits, 2 linhas, fonte 5x8
    lcd_write_command(0x0C); // Display ON, cursor OFF
    lcd_write_command(0x06); // Incrementar cursor
    lcd_clear();
}

void lcd_clear(void)
{
    lcd_write_command(0x01);
    vTaskDelay(pdMS_TO_TICKS(2));
}

void lcd_set_cursor(uint8_t col, uint8_t row)
{
    static const uint8_t row_offsets[] = {0x00, 0x40, 0x14, 0x54};
    lcd_write_command(0x80 | (col + row_offsets[row % 4]));
}

void lcd_write_string_and_temp(const char *str, float value, uint8_t current_unit)
{
    char buffer[32];
    snprintf(buffer, sizeof(buffer), "%s%.1f", str, convert_temperature(value));  
    const char *ptr = buffer;
    while (*ptr) {
        lcd_write_data(*ptr++);
    }

    if(current_unit == TEMPERATURE_CELSIUS || current_unit == TEMPERATURE_FAHRENHEIT){
        lcd_write_data(0xDF);
    } 
    lcd_write_data(*get_unit_label()); // Escreve o símbolo da unidade
}

void lcd_write_only_string(const char *str)
{
    while (*str) {
        lcd_write_data(*str++);
    }
}

void lcd_scroll_text(const char *str, uint8_t row, uint32_t delay_ms)
{
    size_t len = strlen(str);
    char buffer[17]; // LCD 16 colunas + null terminator
    memset(buffer, ' ', sizeof(buffer) - 1);
    buffer[16] = '\0';

    // Escreve o texto completo deslocando letra por letra
    for (int start = 0; start < len + 16; ++start) {
        for (int i = 0; i < 16; ++i) {
            int idx = start - (16 - i);
            buffer[i] = (idx >= 0 && idx < len) ? str[idx] : ' ';
        }

        lcd_set_cursor(0, row);
        lcd_write_only_string(buffer);
        vTaskDelay(pdMS_TO_TICKS(delay_ms));
    }
}


// ========== Funções internas ==========

static void lcd_write_command(uint8_t cmd)
{
    lcd_send(cmd, false);
}

static void lcd_write_data(uint8_t data)
{
    lcd_send(data, true);
}

static void lcd_send(uint8_t data, bool is_data)
{
    LCD_WRITE_PIN(LCD_RS, is_data);
    lcd_write_nibble(data >> 4);
    lcd_write_nibble(data & 0x0F);
}

static void lcd_write_nibble(uint8_t nibble)
{
    LCD_WRITE_PIN(LCD_D4, (nibble >> 0) & 0x01);
    LCD_WRITE_PIN(LCD_D5, (nibble >> 1) & 0x01);
    LCD_WRITE_PIN(LCD_D6, (nibble >> 2) & 0x01);
    LCD_WRITE_PIN(LCD_D7, (nibble >> 3) & 0x01);
    lcd_pulse_enable();
}

static void lcd_pulse_enable(void)
{
    LCD_WRITE_PIN(LCD_EN, 0);
    esp_rom_delay_us(1);
    LCD_WRITE_PIN(LCD_EN, 1);
    esp_rom_delay_us(1);
    LCD_WRITE_PIN(LCD_EN, 0);
    esp_rom_delay_us(100);
}
