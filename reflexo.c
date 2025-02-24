// projeto final: Pedro Henrique Santos Souza Duarte

#include <stdio.h>
#include "pico/stdlib.h"
#include "inc/ssd1306.h"
#include "hardware/i2c.h"
#include "inc/font.h"
#include <string.h>
#include <stdlib.h>
#include "ws2818b.pio.h"
#include "includes/funcoesMatriz.h"
#include <time.h>

// Definições de hardware para comunicação I2C com o display SSD1306
#define I2C_PORT i2c1
#define I2C_SDA 14
#define I2C_SCL 15
#define ENDERECO 0x3C
#define MAX_CARACTERES 16

#define BUZZER_GPIO 21      // Pino do buzzer

// Dimensões da matriz de LEDs
#define MATRIX_ROWS 5
#define MATRIX_COLS 5
#define MATRIX_DEPTH 3

// Definição dos pinos dos botões de entrada
#define BUTTON_A_PIN 5       // Pino do botão A
#define BUTTON_B_PIN 6       // Pino do botão B
#define JOYSTICK_BUTTON 22   // Botão do joystick7

// Variáveis globais de controle do jogo
bool iniciar = false;
bool iniciarDisplay = false;
bool executouJogada = true;
int valorRandom = 0;
int score = 0;
int escolha = 0;
int maiorScore = 0;
char score_str[10]; // Buffer para armazenar o número convertido

// Estrutura do display SSD1306
ssd1306_t ssd;

// Configuração da matriz de LEDs e estrutura de controle
npLED_t leds[LED_COUNT];
int rgb_matrix[MATRIX_ROWS][MATRIX_COLS][MATRIX_DEPTH];

// Controle de debounce para os botões
volatile uint64_t last_interrupt_time = 0;
#define DEBOUNCE_DELAY 300  // 300 ms de debounce

// Protótipos de funções
void drawPattern(int pattern[5][5], npLED_t leds[], int rgb_matrix[MATRIX_ROWS][MATRIX_COLS][MATRIX_DEPTH]);
void a1(npLED_t leds[], int rgb_matrix[MATRIX_ROWS][MATRIX_COLS][MATRIX_DEPTH]);
void a2(npLED_t leds[], int rgb_matrix[MATRIX_ROWS][MATRIX_COLS][MATRIX_DEPTH]);
void iniciarJogo();
void terminarJogo();

// Função de callback para interrupções
typedef void (*AnimationFunction)(npLED_t[], int[MATRIX_ROWS][MATRIX_COLS][MATRIX_DEPTH]);
AnimationFunction animations[] = {a1, a2};

const uint32_t buzzer_frequency = 250;

// Ativa o buzzer com uma certa frequência e por um certo tempo
void buzzer_active(uint32_t buzzer_frequency_in, uint32_t duration_buzzer_on) {
    uint32_t half_period_us = (1000000 / buzzer_frequency_in) / 2; // Define por quanto tempo o pino conectado ao buzzer deve ficar em nível alto/baixo

    // gera uma onda quadrada
    for (uint32_t i = 0; i < duration_buzzer_on * 1000; i += half_period_us * 2) {
        gpio_put(BUZZER_GPIO, 1);
        sleep_us(half_period_us);
        gpio_put(BUZZER_GPIO, 0);
        sleep_us(half_period_us);
    }
}

void gpio_callback(uint gpio, uint32_t events) {
    uint64_t current_time = time_us_64();  // Usa um tipo de dado compatível para evitar overflow
    if (current_time - last_interrupt_time > DEBOUNCE_DELAY * 1000) {
        last_interrupt_time = current_time;

        if(gpio == BUTTON_A_PIN && iniciar){
            escolha = 1;
            executouJogada = true;

            if(valorRandom == escolha){
                score++;
                escolha = 0;
            }
            else{
                executouJogada = false;
            }

        }
        else if(gpio == BUTTON_B_PIN && iniciar){
            escolha = 2;
            executouJogada = true;

            if(valorRandom == escolha){
                score++;
                escolha = 0;
            }
            else{
                executouJogada = false;
            }

        }
        else if(gpio == JOYSTICK_BUTTON){
            iniciarDisplay = true;
            iniciar = true;
            executouJogada = true;
        }

        escolha = 0;
    }
}

int main() {
    stdio_init_all();

    i2c_init(I2C_PORT, 400 * 1000);
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA);
    gpio_pull_up(I2C_SCL);

    ssd1306_init(&ssd, WIDTH, HEIGHT, false, ENDERECO, I2C_PORT);
    ssd1306_config(&ssd);
    ssd1306_send_data(&ssd);
    ssd1306_fill(&ssd, false);
    ssd1306_send_data(&ssd);

    gpio_init(BUTTON_A_PIN);
    gpio_set_dir(BUTTON_A_PIN, GPIO_IN);
    gpio_pull_up(BUTTON_A_PIN);

    gpio_init(BUTTON_B_PIN);
    gpio_set_dir(BUTTON_B_PIN, GPIO_IN);
    gpio_pull_up(BUTTON_B_PIN);

    gpio_init(JOYSTICK_BUTTON);
    gpio_set_dir(JOYSTICK_BUTTON, GPIO_IN);
    gpio_pull_up(JOYSTICK_BUTTON);

    gpio_init(BUZZER_GPIO);
    gpio_set_dir(BUZZER_GPIO, GPIO_OUT);

    gpio_set_irq_enabled(BUTTON_A_PIN, GPIO_IRQ_EDGE_FALL, true);
    gpio_set_irq_enabled(BUTTON_B_PIN, GPIO_IRQ_EDGE_FALL, true);
    gpio_set_irq_enabled(JOYSTICK_BUTTON, GPIO_IRQ_EDGE_FALL, true);
    gpio_set_irq_callback(&gpio_callback);
    irq_set_enabled(IO_IRQ_BANK0, true);

    matrizInit(LED_PIN, leds);

    turnOffLEDs(leds);

    srand(time(NULL));  // Inicializa o gerador de números aleatórios
    while (true) {
        tight_loop_contents();
        exibirScore(score_str);

        if(iniciarDisplay){
            iniciarJogo();
            buzzer_active(buzzer_frequency, 1000);
            iniciarDisplay = false;
        }

        //if para começar o jogo
        if(iniciar){

            if(executouJogada){
                int random_value = (rand() % 2) + 1;  // Gera um número aleatório entre 1 e 2
                valorRandom = random_value;
        
                if (random_value == 1) {
                    animations[0](leds, rgb_matrix);
    
                } else if (random_value == 2) {
                    animations[1](leds, rgb_matrix);
                }

                executouJogada = false;
            }else{
                iniciar = false;
                
                buzzer_active(100, 500);
                buzzer_active(130, 300);
                buzzer_active(100, 300);
                buzzer_active(50, 300);
                terminarJogo();
                sleep_ms(1000);
                if(score > maiorScore){
                    maiorScore = score;
                    sprintf(score_str, "%d", maiorScore); // Converte o inteiro para string
                }

                score = 0;
                
            }
        }

        // ajuste de velociade
        if(score > 5){
            sleep_ms(500);
        }
        else{
            sleep_ms(1000);
        }

        turnOffLEDs(leds);

        if(score > 5){
            sleep_ms(300);
        }
        else{
            sleep_ms(500);
        }

        // Limpar o display após a contagem
        ssd1306_fill(&ssd, false);
        ssd1306_send_data(&ssd);
            
    }
}

void a1(npLED_t leds[], int rgb_matrix[MATRIX_ROWS][MATRIX_COLS][MATRIX_DEPTH]) {
    int pattern[5][5] = {
        {0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0},
        {1, 0, 0, 0, 0},
        {0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0}
    };
    drawPattern(pattern, leds, rgb_matrix);
}

void a2(npLED_t leds[], int rgb_matrix[MATRIX_ROWS][MATRIX_COLS][MATRIX_DEPTH]) {
    int pattern[5][5] = {
        {0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0},
        {0, 0, 0, 0, 1},
        {0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0}
    };
    drawPattern(pattern, leds, rgb_matrix);
}

void drawPattern(int pattern[5][5], npLED_t leds[], int rgb_matrix[MATRIX_ROWS][MATRIX_COLS][MATRIX_DEPTH]) {
    turnOffLEDs(leds);
    sleep_ms(300);
    for (int row = 0; row < MATRIX_ROWS; row++) {
        for (int col = 0; col < MATRIX_COLS; col++) {
            if (pattern[row][col]) {
                rgb_matrix[row][col][0] = 0; // Vermelho
                rgb_matrix[row][col][1] = 0; // Verde
                rgb_matrix[row][col][2] = 80; // Azul
            } else {
                rgb_matrix[row][col][0] = 0;
                rgb_matrix[row][col][1] = 0;
                rgb_matrix[row][col][2] = 0;
            }
        }
    }
    spriteWirite(rgb_matrix, leds);
    matrizWrite(leds);
}

void iniciarJogo() {
    // Exibir "INICIANDO..." e atualizar o display
    ssd1306_fill(&ssd, false);  // Limpa o display antes de exibir
    ssd1306_draw_string(&ssd, "INICIANDO...", 20, 20);
    ssd1306_send_data(&ssd);
    sleep_ms(1000);

    // Exibir contagem regressiva "3"
    ssd1306_fill(&ssd, false);
    ssd1306_draw_string(&ssd, "3", 60, 20);
    ssd1306_send_data(&ssd);
    sleep_ms(1000);

    // Exibir contagem regressiva "2"
    ssd1306_fill(&ssd, false);
    ssd1306_draw_string(&ssd, "2", 60, 20);
    ssd1306_send_data(&ssd);
    sleep_ms(1000);

    // Exibir contagem regressiva "1"
    ssd1306_fill(&ssd, false);
    ssd1306_draw_string(&ssd, "1", 60, 20);
    ssd1306_send_data(&ssd);
    sleep_ms(1000);

    // Limpar o display após a contagem
    ssd1306_fill(&ssd, false);
    ssd1306_send_data(&ssd);
    exibirScore(score_str);
}

void terminarJogo() {
    // Exibir contagem regressiva "1"
    ssd1306_fill(&ssd, false);
    ssd1306_draw_string(&ssd, "GAME OVER", 20, 20);
    ssd1306_send_data(&ssd);
}

void exibirScore(char *MaiorScoreAtual) {
    char score2[10];
    sprintf(score2, "%d", score); // Converte o inteiro para string

    ssd1306_fill(&ssd, false);
    ssd1306_draw_string(&ssd, "Record: ", 10, 10);
    ssd1306_draw_string(&ssd, MaiorScoreAtual, 65, 10);  

    ssd1306_draw_string(&ssd, "Score: ", 10, 50);
    ssd1306_draw_string(&ssd, score2, 60, 50);  
    ssd1306_send_data(&ssd);
}
