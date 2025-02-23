#include <stdio.h>
#include "pico/stdlib.h"
#include "./include/animation.h"
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
uint32_t last_time = 0;                                         // variável para contar o último tempo verificado
// absolute_time_t debounce;
uint16_t vrx_value, vry_value;                                  // variáveis dos valores mai recentes de x e y do joystick
char buffer_umid[16], buffer_chuva[16];                         // Variáveis de buffer para armazenar os valores como string
uint slice_led_r, slice_led_g, slice_led_b;                     // variáveis para armazenar os slices de PWM correspondentes aos LEDs
uint16_t nivel_chuva = 0, nivel_umidade = 0, aceleracao = 0;    // variáveis para armazenar os valores de precipitação, umidade do solo e aceleração do solo

int main() {

    stdio_init_all();

    // Inicialização dos pinos
    setup_joystick();
    button_init(BUTTON_A_PIN);
    pwm_led_setup(RED_LED_PIN, &slice_led_r);    // configura o PWM para o LED vermelho
    pwm_led_setup(GREEN_LED_PIN, &slice_led_g);  // configura o PWM para o LED verde
    pwm_led_setup(BLUE_LED_PIN, &slice_led_b);   // configura o PWM para o LED azul

    // Inicialização do I2C e do display OLED
    i2c_init(I2C_PORT, 400 * 4000);                               // usando-o em 1600KHz
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);                    // set the GPIO pin function to I2C
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);                    // set the GPIO pin function to I2C
    gpio_pull_up(I2C_SDA);                                        // pull up the data line
    gpio_pull_up(I2C_SCL);                                        // pull up the clock line
    ssd1306_init(&ssd, WIDTH, HEIGHT, false, endereco, I2C_PORT); // inicializa o display
    ssd1306_config(&ssd);                                         // configura o display
    ssd1306_send_data(&ssd);                                      // envia os dados para o display

    // debounce = delayed_by_ms(get_absolute_time(), 200); // inicializa o debounce
    
    // Desenha imagem de boas-vindas no display
    ssd1306_draw_bitmap(&ssd, bitmap_128x64);

    // Aguarda o usuário pressionar o botão A (à esquerda) para iniciar o sistema
    while (gpio_get(BUTTON_A_PIN)) {
        sleep_ms(50);  // Pequena espera para evitar uso excessivo de CPU
    }

    // Limpa o display, desenha a borda e uma mensagem de aguardo e envia os dados para serem exibidos
    ssd1306_fill(&ssd, false);
    ssd1306_rect(&ssd, 2, 2, 124, 60, true);
    ssd1306_draw_string(&ssd, "Aguarde um", 28, 19);
    ssd1306_draw_string(&ssd, "momento ...", 20, 35);
    ssd1306_send_data(&ssd);

    while (true) {

        joystick_read_axis(&vrx_value, &vry_value); // lê os valores dos eixos do joystick

        // Umidade baixa: Menos de 20% (baixo risco)
        // Umidade média: Entre 21 e 40% (risco moderado)
        // Umidade alta: Acima de 41-60% (alto risco)
        // Umidade crítica: Acima de 61% (risco crítico)

        nivel_umidade = vrx_value / 40.95; // (4096 / 100%)

        // Chuva fraca: abaixo de 5,0 mm 
        // Chuva moderada: entre 5,0 e 25 mm 
        // Chuva forte: entre 26 e 50 mm 
        // Chuva muito forte: acima de 50,0 e até 150 mm (limite do aparelho)
        
        nivel_chuva = vry_value / 27.3; // (4096 / 150mm)

        snprintf(buffer_umid, sizeof(buffer_umid), "%d%%", nivel_umidade);
        snprintf(buffer_chuva, sizeof(buffer_chuva), "%dmm", nivel_chuva);

        // RISCO BAIXO E LED VERDE
        if (nivel_umidade < 20 || nivel_chuva < 5) {

            // Define risco
            ssd1306_draw_string(&ssd, "Risco Baixo", 20, 46);

            // Define cor do LED
            for (int ciclo_atv = 200; ciclo_atv <= 3000; ciclo_atv += 30) {
                pwm_set_duty_cycle_rgb(ciclo_atv, 13, 201, 0);
                sleep_ms(10);
            }
            for (int ciclo_atv = 3000; ciclo_atv >= 200; ciclo_atv -= 30) {
                pwm_set_duty_cycle_rgb(ciclo_atv, 13, 201, 0);
                sleep_ms(10);
            }

            // // Manda mensagem para a central
            // printf("DADOS COLETADOS\n");
            // printf("Umidade: %d\n", nivel_umidade);
            // printf("Chuva: %d\n", nivel_chuva);
            // printf("Aceleração: %d\n", aceleracao);
        }

        // RISCO MODERADO E LED AMARELO
        if (nivel_umidade > 21 && nivel_umidade < 40 && nivel_chuva > 5 && nivel_chuva < 26) {
            ssd1306_draw_string(&ssd, "Risco Moderado", 20, 46);

            for (int ciclo_atv = 200; ciclo_atv <= 3000; ciclo_atv += 30) {
                pwm_set_duty_cycle_rgb(ciclo_atv, 255, 230, 0);
                sleep_ms(10);
            }
            for (int ciclo_atv = 3000; ciclo_atv >= 200; ciclo_atv -= 30) {
                pwm_set_duty_cycle_rgb(ciclo_atv, 255, 230, 0);
                sleep_ms(10);
            }
        }

        // RISCO ALTO E LED LARANJA
        if (nivel_umidade > 41 && nivel_umidade < 60 && nivel_chuva > 26 && nivel_chuva < 50) {
            ssd1306_draw_string(&ssd, "Risco Moderado", 20, 46);

            for (int ciclo_atv = 200; ciclo_atv <= 3000; ciclo_atv += 30) {
                pwm_set_duty_cycle_rgb(ciclo_atv, 255, 230, 0);
                sleep_ms(10);
            }
            for (int ciclo_atv = 3000; ciclo_atv >= 200; ciclo_atv -= 30) {
                pwm_set_duty_cycle_rgb(ciclo_atv, 255, 230, 0);
                sleep_ms(10);
            }
        }

        // RISCO CRÍTICO E LED VERMELHO
        if (nivel_umidade >= 61 || nivel_chuva > 50) {
            ssd1306_draw_string(&ssd, "Risco Crítico", 20, 46);

            for (int ciclo_atv = 200; ciclo_atv <= 3000; ciclo_atv += 30) {
                pwm_set_duty_cycle_rgb(ciclo_atv, 209, 2, 2);
                sleep_ms(10);
            }
            for (int ciclo_atv = 3000; ciclo_atv >= 200; ciclo_atv -= 30) {
                pwm_set_duty_cycle_rgb(ciclo_atv, 209, 2, 2);
                sleep_ms(10);
            }
        }
        
        // if (vry_value >= 2400 && vry_value <= 2120) {
            //     led_r_level = abs(vrx_value - 2047) * 2;
            // } 
            // else if { // caso contrário, mantém o level em 0
            //     led_r_level = 0;
            // }

        // Imprime um quadro com os dados obtidos no display OLED
        ssd1306_fill(&ssd, false); 
        ssd1306_rect(&ssd, 2, 2, 124, 60, true);   
        ssd1306_draw_string(&ssd, "Umidade: ", 10, 8);
        ssd1306_draw_string(&ssd, buffer_umid, 75, 8);       // Desenha a string no display
        ssd1306_draw_string(&ssd, "Chuva: ", 10, 19);
        ssd1306_draw_string(&ssd, buffer_chuva, 65, 19);     // Desenha a string no display
        ssd1306_draw_string(&ssd, "Desloc: 20m/s@", 10, 30);

        ssd1306_send_data(&ssd);
    }
}
