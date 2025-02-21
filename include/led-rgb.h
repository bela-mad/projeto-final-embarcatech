#ifndef LED_RGB_H
#define LED_RGB_H

// Importações
#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pwm.h"

/*
    Código RGB padrão para os sinais de alerta

    Verde: (13, 201, 0)
    pwm_set_duty_cycle_rgb(ciclo_atv, 13, 201, 0);

    Amarelo: (255, 230, 0)
    pwm_set_duty_cycle_rgb(ciclo_atv, 255, 230, 0);

    Vermelho: (209, 2, 2)
    pwm_set_duty_cycle_rgb(ciclo_atv, 209, 2, 2);
*/ 


// Defines
#define GREEN_LED_PIN 11
#define BLUE_LED_PIN 12
#define RED_LED_PIN 13

// Cabeçalhos das funções
void pwm_led_setup(uint led, uint *slice);
void pwm_set_duty_cycle_rgb(uint ciclo_ativo, uint8_t r, uint8_t g, uint8_t b);

#endif // LED_RGB_H