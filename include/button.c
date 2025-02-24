#include "button.h"

// Funções

/**
 * @brief Inicializa o botão e configura o pino como entrada com pull-up interno.
 *
 * Inicializa o pino do botão especificado e o configura como entrada, com o resistor pull-up 
 * interno habilitado para garantir um estado lógico alto quando o botão não for pressionado.
 *
 * @param gpio O número do pino GPIO ao qual o botão está conectado.
 */
void button_init(uint gpio) {
    gpio_init(gpio);             
    gpio_set_dir(gpio, GPIO_IN); 
    gpio_pull_up(gpio);          
}