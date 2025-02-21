#include <stdio.h>
#include "pico/stdlib.h"
#include "./include/buzzer.h"
#include "./include/font.h"
#include "./include/joystick.h"
#include "./include/led-rgb.h"
#include "./include/microfone.h"
#include "./include/ssd1306.h"

// Declaração de variáveis
uint slice_led_r, slice_led_g, slice_led_b;                       // variáveis para armazenar os slices de PWM correspondentes aos LEDs
uint16_t led_r_level = 0, led_g_level = 0, led_b_level = 0;       // inicialização dos níveis de PWM para os LEDs


int main() {
    stdio_init_all();

    // Inicialização dos pinos
    setup_joystick();
    pwm_led_setup(BLUE_LED_PIN, &slice_led_b); // configura o PWM para o LED azul
    pwm_led_setup(RED_LED_PIN, &slice_led_r);  // configura o PWM para o LED vermelho
    pwm_led_setup(GREEN_LED_PIN, &slice_led_g);  // configura o PWM para o LED vermelho

    while (true) {
        for (int ciclo_atv = 200; ciclo_atv <= 3000; ciclo_atv += 30) {
            pwm_set_duty_cycle_rgb(ciclo_atv, 209, 2, 2);
            sleep_ms(10);
        }
        for (int ciclo_atv = 3000; ciclo_atv >= 200; ciclo_atv -= 30) {
            pwm_set_duty_cycle_rgb(ciclo_atv, 209, 2, 2);
            sleep_ms(10);
        }
    }
}
