#include "buzzer.h"

// Funções

/**
 * @brief Inicializa o buzzer no pino especificado
 * 
 * Configura o pino GPIO fornecido como saída PWM para controlar o buzzer.
 * O PWM é desativado inicialmente até que seja ativado por outra função.
 * 
 * @param gpio Pino GPIO conectado ao buzzer.
 */
uint buzzer_init(uint8_t gpio) {
    gpio_set_function(gpio, GPIO_FUNC_PWM);        
    uint slice = pwm_gpio_to_slice_num(gpio);
    pwm_set_enabled(slice, false);

    return slice;
}

/**
 * @brief Liga o buzzer em uma frequência específica.
 * 
 * Configura e ativa o sinal PWM no pino especificado para gerar um som na frequência desejada.
 * O PWM é configurado com um divisor de clock fixo e um duty cycle de aproximadamente 70%.
 * 
 * @param gpio Pino GPIO conectado ao buzzer.
 * @param freq Frequência do som gerado pelo buzzer (em Hz).
 */
void buzzer_turn_on(uint8_t gpio, uint slice, uint16_t freq) {
    uint clock_div = 4;                             // divisor do clock
    uint sys_clock = clock_get_hz(clk_sys);         // frequência do sistema
    uint wrap = sys_clock / (clock_div * freq) - 1; // calcula o valor de "wrap" para a frequência desejada

    pwm_set_clkdiv(slice, clock_div);              
    pwm_set_wrap(slice, wrap);                    
    pwm_set_gpio_level(gpio, (wrap * 7) / 10);    
    pwm_set_enabled(slice, true);                   // habilita o PWM
}

/**
 * @brief Emite um beep no buzzer por um tempo determinado.
 * 
 * Ativa o buzzer na frequência especificada por um período de tempo em milissegundos (ms).
 * Após o tempo determinado, o buzzer é desligado.
 * 
 * @param gpio Pino GPIO conectado ao buzzer.
 * @param slice Número do slice PWM correspondente ao pino do buzzer.
 * @param freq Frequência do som gerado pelo buzzer (em Hz).
 * @param duration_ms Duração do beep (em milissegundos).
 */
void buzzer_beep(uint8_t gpio, uint slice, uint16_t freq, uint16_t duration_ms) {
    buzzer_turn_on(gpio, slice, freq); 
    sleep_ms(duration_ms);           
    pwm_set_enabled(slice, false);    
}