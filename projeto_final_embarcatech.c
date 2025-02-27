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

// Declaração de variáveis
ssd1306_t ssd;                                                          // variável para a estrutura do display
uint16_t offset;                                                        // variável do offset do microfone
uint16_t volume;                                                        // variável do volume do microfone
uint16_t vrx_value, vry_value;                                          // variáveis dos valores mais recentes de x e y do joystick
uint slice_buzzer, slice_led_r, slice_led_g, slice_led_b;               // variáveis para armazenar os slices de PWM correspondentes ao buzzer e aos LEDs
uint16_t nivel_chuva = 0, nivel_umidade = 0, aceleracao = 0;            // variáveis para armazenar os valores de precipitação, umidade do solo e aceleração do solo
char buffer_umid[8], buffer_chuva[8], buffer_acel[8], buffer_risco[16]; // variáveis de buffer para armazenar os valores como string

int main() {

    stdio_init_all();

    // Inicialização dos pinos
    joystick_setup();
    microfone_setup();
    button_init(BUTTON_A_PIN);
    slice_buzzer = buzzer_init(BUZZER_A_PIN);
    pwm_led_setup(RED_LED_PIN, &slice_led_r);   // configura o PWM para o LED vermelho
    pwm_led_setup(GREEN_LED_PIN, &slice_led_g); // configura o PWM para o LED verde
    pwm_led_setup(BLUE_LED_PIN, &slice_led_b);  // configura o PWM para o LED azul

    // Inicialização do I2C e do display OLED
    i2c_init(I2C_PORT, 400 * 5000);                               // usando-o em 2000KHz
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);                    // set the GPIO pin function to I2C
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);                    // set the GPIO pin function to I2C
    gpio_pull_up(I2C_SDA);                                        // pull up the data line
    gpio_pull_up(I2C_SCL);                                        // pull up the clock line
    ssd1306_init(&ssd, WIDTH, HEIGHT, false, endereco, I2C_PORT); // inicializa o display
    ssd1306_config(&ssd);                                         // configura o display
    ssd1306_send_data(&ssd);                                      // envia os dados para o display

    // Cálculo do offset para o microfone
    offset = microfone_calcula_offset();

    // Desenha imagem de boas-vindas no display
    ssd1306_draw_bitmap(&ssd, bitmap_128x64);

    // Aguarda o usuário pressionar o botão A (à esquerda) para iniciar o sistema
    while (gpio_get(BUTTON_A_PIN)) {
        sleep_ms(50); // pequena espera para evitar uso excessivo de CPU
    }

    // Limpa o display, desenha a borda e uma mensagem de aguardo e envia os dados para serem exibidos
    ssd1306_fill(&ssd, false);
    ssd1306_rect(&ssd, 2, 2, 124, 60, true);
    ssd1306_draw_string(&ssd, "Aguarde um", 28, 19);
    ssd1306_draw_string(&ssd, "momento ...", 20, 35);
    ssd1306_send_data(&ssd);

    while (true) {

        joystick_read_axis(&vrx_value, &vry_value); // lê os valores dos eixos do joystick
        volume = microfone_calcula_volume(offset);

        // Umidade baixa: Menos de 20% (risco baixo)
        // Umidade média: Entre 21 e 40% (risco moderado)
        // Umidade alta: Acima de 41-60% (risco alto)
        // Umidade crítica: Acima de 61% (risco crítico)

        // Umidade do solo
        if (vrx_value < 2100 && vrx_value > 1051) {  // decremento da umidade em 1
            if (nivel_umidade > 0) {
                nivel_umidade--;
            }
        }
        else if (vrx_value <= 1050) { // decremento da umidade em 2
            if (nivel_umidade > 1) {
                nivel_umidade-=2;
            }
        }
        else if (vrx_value > 2350 && vrx_value < 3220) {  // incremento da umidade em 1
            if (nivel_umidade < 100) {
                nivel_umidade++;
            }
        }
        else if (vrx_value >= 3221) {  // incremento da umidade em 2
            if (nivel_umidade < 99) {
                nivel_umidade+=2;
            }
        }

        // Chuva fraca: abaixo de 5,0 mm
        // Chuva moderada: entre 5,0 e 25 mm
        // Chuva forte: entre 26 e 50 mm
        // Chuva muito forte: acima de 50,0 e até 150 mm (limite do aparelho)

        // Precipitação
        if (vry_value < 1850 && vry_value > 901) {  // decremento da chuva em 1
            if (nivel_chuva > 0) {
                nivel_chuva--;
            }
        }
        else if (vry_value <= 900) { // decremento da chuva em 2
            if (nivel_chuva > 1) {
                nivel_chuva-=2;
            }
        }
        else if (vry_value > 2100 && vry_value < 3100) {  // incremento da chuva em 1
            if (nivel_chuva < 150) {
                nivel_chuva++;
            }
        }
        else if (vry_value >= 3101) {  // incremento da chuva em 2
            if (nivel_chuva < 149) {
                nivel_chuva+=2;
            }
        }

        // Deslizamento leve: 1 a 2 m/s²
        // Deslizamento moderado: 2 a 3 m/s²
        // Deslizamento grande: 3 a 5 m/s²
        // Desmoronamento (grande impacto): > 5 m/s²

        aceleracao = volume / 60.23; // (4096 / 68m/s²)

        // Armazena os valores obtidos nos buffers correspondentes
        snprintf(buffer_umid, sizeof(buffer_umid), "%d%%", nivel_umidade);
        snprintf(buffer_chuva, sizeof(buffer_chuva), "%dmm", nivel_chuva);
        snprintf(buffer_acel, sizeof(buffer_acel), "%dm/s@", aceleracao);

        // RISCO CRÍTICO E LED VERMELHO
        if (aceleracao > 5 || nivel_umidade >= 61 || (nivel_umidade >= 41 && nivel_chuva >= 26)) {
            // Armazenamento do tipo de risco no seu buffer
            snprintf(buffer_risco, sizeof(buffer_risco), "Critico");

            // Acionamento do buzzer
            buzzer_beep(BUZZER_A_PIN, slice_buzzer, 2000, 600);

            // Acionamento do LED RGB com ritmação 
            for (int ciclo_atv = 200; ciclo_atv <= 3000; ciclo_atv += 30) {
                pwm_set_duty_cycle_rgb(ciclo_atv, 209, 2, 2);
                sleep_ms(10);
            }
            for (int ciclo_atv = 3000; ciclo_atv >= 200; ciclo_atv -= 30) {
                pwm_set_duty_cycle_rgb(ciclo_atv, 209, 2, 2);
                sleep_ms(10);
            }
        }

        // RISCO BAIXO E LED VERDE
        else if (nivel_umidade <= 20 && nivel_chuva < 5) {
            // Armazenamento do tipo de risco no seu buffer
            snprintf(buffer_risco, sizeof(buffer_risco), "Baixo");

            // Acionamento do LED RGB com ritmação 
            for (int ciclo_atv = 200; ciclo_atv <= 3000; ciclo_atv += 30) {
                pwm_set_duty_cycle_rgb(ciclo_atv, 13, 201, 0);
                sleep_ms(10);
            }
            for (int ciclo_atv = 3000; ciclo_atv >= 200; ciclo_atv -= 30) {
                pwm_set_duty_cycle_rgb(ciclo_atv, 13, 201, 0);
                sleep_ms(10);
            }
        }

        // RISCO MODERADO E LED AMARELO
        else if ((nivel_umidade <= 40 && nivel_chuva < 26) || (nivel_umidade >= 21 && nivel_umidade <= 40 && nivel_chuva >= 26)) {
            // Armazenamento do tipo de risco no seu buffer
            snprintf(buffer_risco, sizeof(buffer_risco), "Moderado");

            // Acionamento do LED RGB com ritmação 
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
        else {
            // Armazenamento do tipo de risco no seu buffer
            snprintf(buffer_risco, sizeof(buffer_risco), "Alto");

            // Acionamento do LED RGB com ritmação 
            for (int ciclo_atv = 200; ciclo_atv <= 3000; ciclo_atv += 30) {
                pwm_set_duty_cycle_rgb(ciclo_atv, 207, 80, 6);
                sleep_ms(10);
            }
            for (int ciclo_atv = 3000; ciclo_atv >= 200; ciclo_atv -= 30) {
                pwm_set_duty_cycle_rgb(ciclo_atv, 207, 80, 6);
                sleep_ms(10);
            }
        }

        // Imprime um quadro com os dados obtidos no display OLED
        ssd1306_fill(&ssd, false);
        ssd1306_rect(&ssd, 2, 2, 124, 60, true);
        ssd1306_draw_string(&ssd, "Umidade: ", 10, 8);
        ssd1306_draw_string(&ssd, buffer_umid, 75, 8);   
        ssd1306_draw_string(&ssd, "Chuva: ", 10, 19);
        ssd1306_draw_string(&ssd, buffer_chuva, 58, 19);
        ssd1306_draw_string(&ssd, "Desloc: ", 10, 30);
        ssd1306_draw_string(&ssd, buffer_acel, 68, 30);
        ssd1306_draw_string(&ssd, "Risco ", 12, 46);
        ssd1306_draw_string(&ssd, buffer_risco, 58, 46);

        ssd1306_send_data(&ssd);
    }
}
