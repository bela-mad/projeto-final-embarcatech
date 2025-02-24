#include "microfone.h"

// Funções

/**
 * @brief Inicializa o módulo ADC e configura o pino do microfone para entrada.
 *
 * Configura o pino do microfone para entrada ADC e inicializa 
 * o módulo ADC para realizar a leitura dos dados do microfone.
 */
void microfone_setup() {
    adc_init();
    adc_gpio_init(MIC_PIN);
}

/**
 * @brief Calcula o offset do microfone.
 *
 * Realiza múltiplas leituras do microfone e calcula a média dos valores lidos.
 * O valor médio obtido é usado como o "offset" do microfone, que pode ser utilizado
 * para compensar variações do sinal de fundo.
 *
 * @return O valor médio dos 100 valores lidos, representando o offset do microfone.
 */
uint16_t microfone_calcula_offset() {
    uint32_t soma = 0;
    adc_select_input(2); 
    for (int i = 0; i < 100; i++) {
        soma += adc_read();
        sleep_ms(1);
    }
    return soma / 100; 
}

/**
 * @brief Calcula o volume do microfone com base no offset.
 *
 * Calcula o volume do microfone, subtraindo o offset de cada leitura
 * e somando as variações absolutas para obter um valor médio de volume.
 *
 * @param offset O valor de offset previamente calculado para compensar ruídos.
 * 
 * @return O volume do microfone, representado pela média das variações absolutas
 *         das leituras do microfone.
 */
uint16_t microfone_calcula_volume(uint16_t offset) {
    uint32_t soma = 0;
    adc_select_input(2); 
    for (int i = 0; i < 100; i++) {
        int16_t variacao = adc_read() - offset;
        soma += abs(variacao);
        sleep_us(200);   // pequeno atraso para evitar leituras redundantes
    }
    return soma / 100; 
}
