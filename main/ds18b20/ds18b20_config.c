#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"

#include "owb.h"
#include "owb_rmt.h"
#include "ds18b20.h"
#include "ds18b20_config.h"
#include "config.h"

static OneWireBus *owb = NULL;
static DS18B20_Info *ds18b20 = NULL;

void ds18b20_sensor_init()
{
    vTaskDelay(pdMS_TO_TICKS(2000));  // estabilização
    printf("Inicializando sensor DS18B20...\n");

    static owb_rmt_driver_info rmt_driver_info;
    owb = owb_rmt_initialize(&rmt_driver_info, CONFIG_ONE_WIRE_GPIO, RMT_CHANNEL_1, RMT_CHANNEL_0);
    owb_use_crc(owb, true);

    ds18b20 = ds18b20_malloc();
    ds18b20_init_solo(ds18b20, owb);
    ds18b20_use_crc(ds18b20, true);
    ds18b20_set_resolution(ds18b20, DS18B20_RESOLUTION);

    bool parasitic_power = false;
    ds18b20_check_for_parasite_power(owb, &parasitic_power);
    owb_use_parasitic_power(owb, parasitic_power);

    printf("Sensor DS18B20 pronto.\n");
}

float ds18b20_read()
{
    if (!ds18b20 || !owb) {
        printf("Erro: sensor não inicializado!\n");
        return -1000.0f;  // valor inválido
    }

    ds18b20_convert_all(owb);
    ds18b20_wait_for_conversion(ds18b20);

    float temp_c = 0;
    DS18B20_ERROR err = ds18b20_read_temp(ds18b20, &temp_c);
    if (err != DS18B20_OK) {
        printf("Erro ao ler temperatura: %d\n", err);
        return -1000.0f;
    }

    // printf("Temperatura lida: %.2f °C\n", temp_c); 

    return temp_c;
}
