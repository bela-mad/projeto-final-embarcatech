#include "joystick.h"

// Funções

/**
 * @brief Inicializa o módulo ADC e configura os pinos do joystick (VRX e VRY).
 *
 * Inicializa o módulo ADC do microcontrolador e configura os pinos responsáveis 
 * pelos eixos X (VRX) e Y (VRY) do joystick para leitura de valores analógicos.
 * 
 */
void joystick_setup() {
    adc_init();         
    adc_gpio_init(VRX); 
    adc_gpio_init(VRY); 
}

/**
 * @brief Lê os valores dos eixos X e Y do joystick.
 *
 * Realiza a leitura dos valores analógicos dos eixos X e Y do joystick usando o módulo ADC, 
 * de acordo com o canal. Os valores lidos são armazenados nas variáveis passadas como
 * argumento (vrx_value e vry_value).
 *
 * @param vrx_value Ponteiro para a variável que armazenará o valor do eixo X.
 * @param vry_value Ponteiro para a variável que armazenará o valor do eixo Y.
 */
void joystick_read_axis(uint16_t *vrx_value, uint16_t *vry_value) {
    
    adc_select_input(ADC_CHANNEL_0); 
    sleep_us(2);                     // pequeno delay para estabilidade
    *vrx_value = adc_read();         
    
    adc_select_input(ADC_CHANNEL_1); 
    sleep_us(2);                     // pequeno delay para estabilidade
    *vry_value = adc_read();         
}