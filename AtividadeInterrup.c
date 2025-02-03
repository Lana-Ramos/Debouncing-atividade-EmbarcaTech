// Bibliotecas necessárias
#include <stdio.h>
#include <stdlib.h>
#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "hardware/clocks.h"
#include "ws2812.pio.h"

// Definição de constantes da matriz de leds
#define IS_RGBW false
#define NUM_PIXELS 25
#define WS2812_PIN 7
#define tempo 400

// Variáveis globais para armazenar a cor
uint8_t led_r = 50;
uint8_t led_g = 0;
uint8_t led_b = 0;

// Definição dos pinos dos leds e botões
#define LED_G_PIN 11
#define LED_B_PIN 12
#define LED_R_PIN 13

#define BUTTON_A_PIN 5
#define BUTTON_B_PIN 6

// Variáveis de controle de estado
static volatile int numero_atual = 0; // Número exibido na matriz de LEDs
static uint32_t last_interrupt_time_a = 0; // Armazena o último tempo de interrupção do botão A
static uint32_t last_interrupt_time_b = 0; // Armazena o último tempo de interrupção do botão B

// Buffer para armazenar quais LEDs estão ligados matriz 5x5 e formar os números de 0 a 9
bool led_buffer[10][NUM_PIXELS] = {
   //1, 2, 3, 4, 5, 6, 7, 8, 9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25}
    {1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 0, 0, 0, 1, 1, 0, 0, 0, 1, 1, 1, 1, 1, 1}, // Número 0
    {0, 1, 1, 1, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 1, 0, 0}, // Número 1
    {1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1}, // Número 2
    {1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1}, // Número 3
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 0, 0, 0, 1}, // Número 4
    {1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 1}, // Número 5
    {1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 1}, // Número 6
    {0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 1, 1, 1, 1, 1}, // Número 7
    {1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 1, 1}, // Número 8
    {1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 1, 1}, // Número 9
};

// Função para enviar dados para o LED WS2812
static inline void put_pixel(uint32_t pixel_grb)
{
    pio_sm_put_blocking(pio0, 0, pixel_grb << 8u);
}

// Função para converter valores RGB em um único valor de 32 bits no formato GRB
static inline uint32_t urgb_u32(uint8_t r, uint8_t g, uint8_t b)
{
    return ((uint32_t)(r) << 8) | ((uint32_t)(g) << 16) | (uint32_t)(b);
}

// Função para acender um número específico na matriz de LEDs
void set_one_led(uint8_t r, uint8_t g, uint8_t b, int numero) {

    uint32_t color = urgb_u32(r, g, b); // Define a cor

    for (int i = 0; i < NUM_PIXELS; i++) {
        if (led_buffer[numero][i]) {
            put_pixel(color); // Liga os LEDs
        } else {
            put_pixel(0);  // Desliga os LEDs
        }
    }
}

// Função para piscar o LED vermelho 5 vezes por segundo, pois essa ação de blinkar durará 200ms, ou seja, isso se repetirá 5 vezes em 1 segundo.
void blink_red() {
    gpio_put(LED_R_PIN, 1);
    sleep_ms(100);
    gpio_put(LED_R_PIN, 0);
    sleep_ms(100);
}

// Protótipo de função da interrução
static void gpio_irq_handler(uint gpio, uint32_t events);

//Função principal
int main()
{
    PIO pio = pio0;
    int sm = 0;
    uint offset = pio_add_program(pio, &ws2812_program);
    inicializar_componentes();
    ws2812_program_init(pio, sm, offset, WS2812_PIN, 800000, IS_RGBW);

    // Configura interrupções para os botões
    gpio_set_irq_enabled_with_callback(BUTTON_A_PIN, GPIO_IRQ_EDGE_FALL, true, &gpio_irq_handler);
    gpio_set_irq_enabled_with_callback(BUTTON_B_PIN, GPIO_IRQ_EDGE_FALL, true, &gpio_irq_handler);

    // Rotina ou Loop principal
    while (true)
    {
        blink_red(); //Pisca o led de forma contínua, de maneira que pisca 5 vezes por segundo
        set_one_led(led_r, led_g, led_b, numero_atual); // Passa o número atual para exibir
    }

    return 0;
}


static volatile uint a = 1;
static volatile uint32_t last_time = 0;

 // Interrupção dos botões com debouncing
static void gpio_irq_handler(uint gpio, uint32_t events)
{
    uint32_t current_time = to_us_since_boot(get_absolute_time());

    if (gpio == BUTTON_A_PIN && current_time - last_interrupt_time_a > 300000){ //Tempo de Debounce: 300 ms
        last_interrupt_time_a = current_time;
        numero_atual = (numero_atual + 1) % 10; // Incrementa com rolação entre 0-9
        printf("Número exibido: %d\n", numero_atual);
    }
    else if (gpio == BUTTON_B_PIN && current_time - last_interrupt_time_b > 300000) { //Tempo de Debounce: 300 ms
        last_interrupt_time_b = current_time;
        numero_atual = (numero_atual - 1 + 10) % 10; // Decrementa com rolação entre 0-9
        printf("Número exibido: %d\n", numero_atual);
    }
}