#ifndef PELTIER_CONFIG_H
#define PELTIER_CONFIG_H

#include "driver/gpio.h"

void peltier1_on(int time_on);
void peltier2_on(int time_on);

typedef enum {
    PELTIER_OFF,
    PELTIER_1_ON,
    PELTIER_2_ON
} PeltierState;

extern PeltierState peltier_state;

#endif
