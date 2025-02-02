#include <stdio.h>


#define MATRIZ_LEDS 7; // Pino que a matriz de leds está conectada
#define N_LEDS 25; // número de leds na matrix 5x5

// Estrutura que armazena as cores RGB
typedef struct 
{
    uint8_t G,R,B;
}npLED_t;

npLED_t leds[N_LEDS];
void inicializar_leds();
void blink_red();

