#include <stdio.h>
#include "pico/stdlib.h"
#include "./include/button.h"
#include "./include/buzzer.h"
#include "./include/font.h"
#include "./include/joystick.h"
#include "./include/led-rgb.h"
#include "./include/microfone.h"
#include "./include/ssd1306.h"

// Defines
#define I2C_PORT i2c1
#define I2C_SDA 14
#define I2C_SCL 15
#define endereco 0x3C

// Declaração de variáveis
ssd1306_t ssd;
uint32_t last_time = 0;                              // variável para contar o último tempo verificado
// absolute_time_t debounce;
uint16_t vrx_value, vry_value;                       // variáveis dos valores mai recentes de x e y do joystick
uint slice_led_r, slice_led_g, slice_led_b;          // variáveis para armazenar os slices de PWM correspondentes aos LEDs

int main() {

    stdio_init_all();

    // Inicialização dos pinos
    setup_joystick();
    button_init(BUTTON_A_PIN);
    pwm_led_setup(RED_LED_PIN, &slice_led_r);  // configura o PWM para o LED vermelho
    pwm_led_setup(GREEN_LED_PIN, &slice_led_g);  // configura o PWM para o LED verde
    pwm_led_setup(BLUE_LED_PIN, &slice_led_b); // configura o PWM para o LED azul

    // Inicialização do I2C e do display OLED
    i2c_init(I2C_PORT, 400 * 1000);                               // usando-o em 400KHz
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);                    // set the GPIO pin function to I2C
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);                    // set the GPIO pin function to I2C
    gpio_pull_up(I2C_SDA);                                        // pull up the data line
    gpio_pull_up(I2C_SCL);                                        // pull up the clock line
    ssd1306_init(&ssd, WIDTH, HEIGHT, false, endereco, I2C_PORT); // inicializa o display
    ssd1306_config(&ssd);                                         // configura o display
    ssd1306_send_data(&ssd);                                      // envia os dados para o display

    // debounce = delayed_by_ms(get_absolute_time(), 200); // inicializa o debounce
    
    // Limpa o display, desenha a borda e a mensagem de boas-vindas e envia os dados para serem exibidos
    ssd1306_fill(&ssd, false);
    ssd1306_rect(&ssd, 2, 2, 124, 60, true);
    ssd1306_draw_string(&ssd, "Bem-vindo", 30, 8);
    ssd1306_draw_string(&ssd, "ao sistema", 28, 19);
    ssd1306_draw_string(&ssd, "Pressione A", 20, 35);
    ssd1306_draw_string(&ssd, "para iniciar", 18, 46);
    ssd1306_send_data(&ssd);

    // Aguarda o usuário pressionar o botão A (à esquerda) para iniciar o sistema
    while (gpio_get(BUTTON_A_PIN)) {
        sleep_ms(50);  // Pequena espera para evitar uso excessivo de CPU
    }

    // Limpa o display, desenha a borda e a mensagem de boas-vindas e envia os dados para serem exibidos
    ssd1306_fill(&ssd, false);
    ssd1306_rect(&ssd, 2, 2, 124, 60, true);
    ssd1306_send_data(&ssd);

    while (true) {

        joystick_read_axis(&vrx_value, &vry_value); // lê os valores dos eixos do joystick






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
