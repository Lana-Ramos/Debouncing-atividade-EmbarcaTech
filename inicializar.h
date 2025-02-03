#include <stdio.h>
#include <stdlib.h>
#include "pico/stdlib.h"

// Definição dos pinos dos leds e botões
#define LED_G_PIN 11
#define LED_B_PIN 12
#define LED_R_PIN 13

#define BUTTON_A_PIN 5
#define BUTTON_B_PIN 6

// Função para inicializar os LEDs e botões
void inicializar_componentes() {
    gpio_init(LED_G_PIN);
    gpio_set_dir(LED_G_PIN, GPIO_OUT);
    gpio_put(LED_G_PIN, 0);

    gpio_init(LED_B_PIN);
    gpio_set_dir(LED_B_PIN, GPIO_OUT);
    gpio_put(LED_B_PIN, 0);

    gpio_init(LED_R_PIN);
    gpio_set_dir(LED_R_PIN, GPIO_OUT);
    gpio_put(LED_R_PIN, 0);

    gpio_init(BUTTON_A_PIN);
    gpio_set_dir(BUTTON_A_PIN, GPIO_IN);
    gpio_pull_up(BUTTON_A_PIN);

    gpio_init(BUTTON_B_PIN);
    gpio_set_dir(BUTTON_B_PIN, GPIO_IN);
    gpio_pull_up(BUTTON_B_PIN);
}