#include "led-rgb.h"

// Declaração de variáveis
const float PWM_DIVISOR = 125.0;
const uint16_t WRAP_PERIOD = 19999;
const float frequencia = 50.0;      

// Funções

/**
 * @brief Configura o PWM para controle de intensidade do LED.
 *
 * Configura o pino do LED especificado para operação com PWM, obtém o slice associado 
 * ao pino e ajusta o divisor de clock e o valor do contador para definir o período do PWM.
 *
 * @param led O número do pino GPIO ao qual o LED está conectado.
 * @param slice Ponteiro para a variável onde será armazenado o número do slice
 *              de PWM associado ao pino do LED.
 */
void pwm_led_setup(uint led, uint *slice) {
    gpio_set_function(led, GPIO_FUNC_PWM); 
    *slice = pwm_gpio_to_slice_num(led);  
    pwm_set_clkdiv(*slice, PWM_DIVISOR);  
    pwm_set_wrap(*slice, WRAP_PERIOD);    
    pwm_set_enabled(*slice, true);    
}

/**
 * @brief Modifica o duty cycle do PWM para controlar a intensidade de cada cor do LED RGB.
 *
 * Ajusta os valores do PWM para os pinos de LED RGB (vermelho, verde e azul)
 * de acordo com os valores de intensidade das cores (r, g, b) fornecidos, 
 * calculando o nível do PWM com base no código RGB e no ciclo ativo.
 *
 * @param ciclo_ativo O valor do ciclo ativo do PWM.
 * @param r O valor da intensidade da cor vermelha (0-255).
 * @param g O valor da intensidade da cor verde (0-255).
 * @param b O valor da intensidade da cor azul (0-255).
 */
void pwm_set_duty_cycle_rgb(uint ciclo_ativo, uint8_t r, uint8_t g, uint8_t b) {

    uint32_t level_r = (((r * WRAP_PERIOD) / 255) * ciclo_ativo) / (1000000 / frequencia);
    uint32_t level_g = (((g * WRAP_PERIOD) / 255) * ciclo_ativo) / (1000000 / frequencia);
    uint32_t level_b = (((b * WRAP_PERIOD) / 255) * ciclo_ativo) / (1000000 / frequencia);

    pwm_set_gpio_level(RED_LED_PIN, level_r);
    pwm_set_gpio_level(GREEN_LED_PIN, level_g);
    pwm_set_gpio_level(BLUE_LED_PIN, level_b);
}
