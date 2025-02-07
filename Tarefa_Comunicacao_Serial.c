#include <stdio.h>
#include <stdlib.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "inc/ssd1306.h"
#include "inc/font.h"
#include "hardware/pio.h"
#include "hardware/clocks.h"
#include "ws2812.pio.h"


//Definição de pinos e variáveis globais
#define led_pin_blue 12
#define led_pin_green 11
#define led_pin_red 13
#define button_a 5
#define button_b 6

#define led_matrix_pin 7
#define NUM_LEDS 25

#define IS_RGBW false
static volatile uint32_t last_time = 0;
uint32_t leds[NUM_LEDS];

#define IS_RGBW false
#define I2C_PORT i2c1
#define I2C_SDA 14
#define I2C_SCL 15
#define endereco 0x3C

ssd1306_t ssd; 

//Função para localizar LEDs da matriz através de linhas e colunas
uint8_t localizar_led_xy(uint8_t x, uint8_t y) {
    return (4 - y) * 5 + x;
}

//Gerar números na matriz
const uint8_t numbers[10][5][5] = {
    { // Número 0
        {1, 1, 1, 1, 1},
        {1, 0, 0, 0, 1},
        {1, 0, 0, 0, 1},
        {1, 0, 0, 0, 1},
        {1, 1, 1, 1, 1}
    },
    { //Número 1
        {0, 0, 1, 0, 0},
        {0, 1, 1, 0, 0},
        {0, 0, 1, 0, 0},
        {0, 0, 1, 0, 0},
        {0, 1, 1, 1, 0}
    },
    { // Número 2
        {1, 1, 1, 1, 1},
        {0, 0, 0, 0, 1},
        {1, 1, 1, 1, 1},
        {1, 0, 0, 0, 0},
        {1, 1, 1, 1, 1}
    },
    { //Número 3
        {1, 1, 1, 1, 1},
        {0, 0, 0, 0, 1},
        {1, 1, 1, 1, 1},
        {0, 0, 0, 0, 1},
        {1, 1, 1, 1, 1}
    },
    { //Número 4
        {1, 0, 0, 0, 1},
        {1, 0, 0, 0, 1},
        {1, 1, 1, 1, 1},
        {0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0}
    },
    { // Número 5
        {1, 1, 1, 1, 1},
        {1, 0, 0, 0, 0},
        {1, 1, 1, 1, 1},
        {0, 0, 0, 0, 1},
        {1, 1, 1, 1, 1}
    },
    { // Número 6 
        {1, 1, 1, 1, 1},
        {1, 0, 0, 0, 0},
        {1, 1, 1, 1, 1},
        {1, 0, 0, 0, 1},
        {1, 1, 1, 1, 1}
    },
    { //Número 7
        {1, 1, 1, 1, 1},
        {0, 0, 0, 0, 1},
        {0, 1, 0, 0, 0},
        {0, 0, 1, 0, 0},
        {0, 0, 0, 1, 0}
    },
    {// Número 8
        {1, 1, 1, 1, 1},
        {1, 0, 0, 0, 1},
        {1, 1, 1, 1, 1},
        {1, 0, 0, 0, 1},
        {1, 1, 1, 1, 1}
    },
    { // Número 9
        {1, 1, 1, 1, 1},
        {1, 0, 0, 0, 1},
        {1, 1, 1, 1, 1},
        {0, 0, 0, 0, 1},
        {1, 1, 1, 1, 1}
    }
};

// Função para criar uma cor GRB na matriz
uint32_t create_color(uint8_t green, uint8_t red, uint8_t blue) {
    return ((uint32_t)green << 16) | ((uint32_t)red << 8) | blue;
}

// Função para exibir um número na matriz
void display_number(uint32_t leds[NUM_LEDS], uint8_t number) {
    for (uint8_t y = 0; y < 5; y++) {
        for (uint8_t x = 0; x < 5; x++) {
            if (numbers[number][y][x]) {
                leds[localizar_led_xy(x, y)] = create_color(0, 50, 50);
            } else {
                leds[localizar_led_xy(x, y)] = 0; 
            }
        }
    }
}

void update_leds(PIO pio, uint sm) {
    for (int i = 0; i < NUM_LEDS; i++) {
        pio_sm_put_blocking(pio, sm, leds[i] << 8u);
    }
}

// Prototipo da função de interrupção
static void gpio_irq_handler(uint gpio, uint32_t events);

//Função para iniciar display
void init_display() {
    i2c_init(I2C_PORT, 400 * 1000);
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA);
    gpio_pull_up(I2C_SCL);
    ssd1306_init(&ssd, 128, 64, false, endereco, I2C_PORT);
    ssd1306_config(&ssd);
    ssd1306_fill(&ssd, false);
    ssd1306_send_data(&ssd);
}

int main()
{
    stdio_init_all();

     //Inicialização e configuração inicial de GPIOs simples
     gpio_init(led_pin_blue);
     gpio_set_dir(led_pin_blue, GPIO_OUT);
     gpio_put(led_pin_blue, 0);
 
     gpio_init(led_pin_green);
     gpio_set_dir(led_pin_green, GPIO_OUT);
     gpio_put(led_pin_green, 0);

     gpio_init(led_pin_red);
     gpio_set_dir(led_pin_red, GPIO_OUT);
     gpio_put(led_pin_red, 0);
 
     gpio_init(led_matrix_pin);
     gpio_set_dir(led_matrix_pin, GPIO_OUT);
 
     gpio_init(button_a);
     gpio_set_dir(button_a, GPIO_IN);
     gpio_pull_up(button_a);
 
     gpio_init(button_b);
     gpio_set_dir(button_b, GPIO_IN);
     gpio_pull_up(button_b);
 
     // Configurações PIO
     PIO pio = pio0;
     int sm = 0;
     uint offset = pio_add_program(pio, &ws2812_program);
     printf("Loaded program at %d\n", offset);
 
     ws2812_program_init(pio, sm, offset, led_matrix_pin, 800000, IS_RGBW);
 
      for (int i = 0; i < NUM_LEDS; i++) {
         leds[i] = 0; 
     }
     update_leds(pio, sm);

    //Configurações do Display
    init_display();

    // Configuração da interrupções
    gpio_set_irq_enabled_with_callback(button_a, GPIO_IRQ_EDGE_FALL, true, &gpio_irq_handler);
    gpio_set_irq_enabled_with_callback(button_b, GPIO_IRQ_EDGE_FALL, true, &gpio_irq_handler);

    while (true) {
        ssd1306_fill(&ssd, false);
        if (stdio_usb_connected()) {
            char c;
            if (scanf("%c", &c) == 1) {
                if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z')) {
                    ssd1306_fill(&ssd, false);  // Limpa o display
                    ssd1306_draw_char(&ssd, c, 60, 28);  // Centraliza o caractere
                    ssd1306_send_data(&ssd);  // Atualiza o display
                    printf("Exibindo letra: %c\n", c);
                    } else if (c >= '0' && c <= '9') {
                        int number = c - '0';
                        
                        // Exibir o número na matriz de LED
                        display_number(leds, number);
                        update_leds(pio, sm);
                }
        sleep_ms(500);
            }
        }
    }
}
//Funções de interrupção
void gpio_irq_handler(uint gpio, uint32_t events)
{
    #define DEBOUNCE_TIME_MS 50
    static uint32_t last_a_time = 0;
    static uint32_t last_b_time = 0;
    char buffer[20];
    uint32_t now = to_ms_since_boot(get_absolute_time());

    ssd1306_fill(&ssd, false);
    
    if (gpio == button_a) {
        if ((now - last_a_time) > DEBOUNCE_TIME_MS) {
            last_a_time = now;
            bool estado = gpio_get(led_pin_green);
            gpio_put(led_pin_green, !estado);
            snprintf(buffer, "Verde: %s", estado ? "Verde OFF" : "Verde ON");
            ssd1306_draw_string(&ssd, buffer, 30, 30);
            printf("LED Verde: %s\n", estado ? "Desligado" : "Ligado");
        }
    } 
    else if (gpio == button_b) {
        if ((now - last_b_time) > DEBOUNCE_TIME_MS) {
            last_b_time = now;
            bool estado = gpio_get(led_pin_blue);
            gpio_put(led_pin_blue, !estado);
            snprintf(buffer, "Azul: %s", estado ? "Azul OFF" : "Azul ON");
            ssd1306_draw_string(&ssd, buffer, 30, 30);
            printf("LED Azul: %s\n", estado ? "Desligado" : "Ligado");
        }
    }
    ssd1306_send_data(&ssd);
}
