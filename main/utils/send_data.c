#include "send_data.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "config.h"
#include <stdio.h>
#include <string.h>


void send_data_sdout(float temperature, float set_point)
{
    printf("%.2f,%.2f,%d\n", temperature, set_point, SAMPLING_RATE);
}