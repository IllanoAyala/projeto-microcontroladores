#include "convert_temperature.h"
#include "config.h"

TemperatureUnit current_unit = TEMPERATURE_CELSIUS;

float convert_temperature(float celsius) {
    switch (current_unit) {
        case TEMPERATURE_CELSIUS:
            return celsius;
        case TEMPERATURE_FAHRENHEIT:
            return celsius * 9.0f / 5.0f + 32.0f;
        case TEMPERATURE_KELVIN:
            return celsius + 273.15f;
        default:
            return celsius; 
    }
}

const char* get_unit_label() {
    switch (current_unit) {
        case TEMPERATURE_CELSIUS:
            return "C";
        case TEMPERATURE_FAHRENHEIT:
            return "F";
        case TEMPERATURE_KELVIN:
            return "K";
        default:
            return "";
    }
}
