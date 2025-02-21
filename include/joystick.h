#ifndef JOYSTICK_H
#define JOYSTICK_H

#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/adc.h"

// Defines
#define VRY 26      
#define VRX 27        
#define ADC_CHANNEL_0 0
#define ADC_CHANNEL_1 1

// Cabeçalhos das funções
void setup_joystick();
void joystick_read_axis(uint16_t *vrx_value, uint16_t *vry_value);

#endif