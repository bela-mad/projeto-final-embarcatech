#include "led-rgb.h"

// Declaração de variáveis
const float PWM_DIVISOR = 125.0;
const uint16_t WRAP_PERIOD = 19999;
const float frequencia = 50.0;      

// Funções

// Função para configurar o PWM de um LED
void pwm_led_setup(uint led, uint *slice) {
    gpio_set_function(led, GPIO_FUNC_PWM); // configura o pino do LED como saída PWM
    *slice = pwm_gpio_to_slice_num(led);   // obtém o slice do PWM associado ao pino do LED
    pwm_set_clkdiv(*slice, PWM_DIVISOR);   // define o divisor de clock do PWM
    pwm_set_wrap(*slice, WRAP_PERIOD);     // configura o valor máximo do contador (período do PWM)
    pwm_set_enabled(*slice, true);         // habilita o PWM no slice correspondente ao LED
}

// Função que modifica o duty cycle do PWM de acordo com o código RGB recebido
void pwm_set_duty_cycle_rgb(uint ciclo_ativo, uint8_t r, uint8_t g, uint8_t b) {

    uint32_t level_r = (((r * WRAP_PERIOD) / 255) * ciclo_ativo) / (1000000 / frequencia);
    uint32_t level_g = (((g * WRAP_PERIOD) / 255) * ciclo_ativo) / (1000000 / frequencia);
    uint32_t level_b = (((b * WRAP_PERIOD) / 255) * ciclo_ativo) / (1000000 / frequencia);

    pwm_set_gpio_level(RED_LED_PIN, level_r);
    pwm_set_gpio_level(GREEN_LED_PIN, level_g);
    pwm_set_gpio_level(BLUE_LED_PIN, level_b);
}
