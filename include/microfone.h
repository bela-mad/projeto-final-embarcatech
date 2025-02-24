#ifndef MICROFONE_H
#define MICROFONE_H

#include <stdio.h>
#include <stdlib.h>
#include "pico/stdlib.h"
#include "hardware/adc.h"

// Defines
#define MIC_PIN 28  
#define TAMANHO_FILTRO 10  // Tamanho da média móvel

// Cabeçalhos das funções
void microfone_setup();
uint16_t microfone_calcula_offset();
uint16_t microfone_calcula_volume(uint16_t offset);

#endif