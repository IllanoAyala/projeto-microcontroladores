#ifndef CONVERT_TEMPERATURE_H
#define CONVERT_TEMPERATURE_H

#include <stdint.h> 
#include <stdio.h>

typedef enum {
    TEMPERATURE_CELSIUS,
    TEMPERATURE_FAHRENHEIT,
    TEMPERATURE_KELVIN,
    TEMPERATURE_UNIT_COUNT 
} TemperatureUnit;

extern TemperatureUnit current_unit;

float convert_temperature(float celsius);

const char* get_unit_label();

#endif 
