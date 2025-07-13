#include "peltier_config.h"
#include "config.h"
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

PeltierState peltier_state = PELTIER_OFF;

void peltier1_on(int time_on) {
    gpio_config_t conf = {
        .mode = GPIO_MODE_OUTPUT,
        .pin_bit_mask = (1ULL << PELTIER_1),
    };
    gpio_config(&conf);
    gpio_set_level(PELTIER_1, 1); // Liga o Peltier 1
    vTaskDelay(pdMS_TO_TICKS(time_on)); // Espera o tempo especificado
    gpio_set_level(PELTIER_1, 0); // Desliga o Peltier 1
}

void peltier2_on(int time_on) {
    gpio_config_t conf = {
        .mode = GPIO_MODE_OUTPUT,
        .pin_bit_mask = (1ULL << PELTIER_2),
    };
    gpio_config(&conf);
    gpio_set_level(PELTIER_2, 1); // Liga o Peltier 2
    vTaskDelay(pdMS_TO_TICKS(time_on)); // Espera o tempo especificado
    gpio_set_level(PELTIER_2, 0); // Desliga o Peltier 2
}



