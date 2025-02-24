# Jogo de Reflexos com Raspberry Pi Pico

## Descrição do Projeto
Este é um jogo de reflexos desenvolvido para a **Raspberry Pi Pico**, utilizando uma matriz de LEDs, botões e um display OLED SSD1306. O jogo desafia o jogador a reagir rapidamente a padrões de LED acesos, pressionando o botão correto no menor tempo possível. O objetivo é atingir a maior pontuação possível antes de cometer um erro.

## Funcionalidades
- **Matriz de LEDs 5x5**: Exibe padrões que o jogador deve identificar.
- **Display OLED SSD1306**: Exibe mensagens do jogo, pontuação e status.
- **Botões de Entrada**: Permitem ao jogador interagir e tomar decisões.
- **Buzzer**: Indica som de acerto e erro.
- **Sistema de Pontuação**: Registra a pontuação e o recorde do jogador.
- **Aumento de Dificuldade**: Com o avanço no jogo, o tempo para reagir diminui.

## Hardware Utilizado
- **Raspberry Pi Pico**
- **Matriz de LEDs WS2812 (5x5)**
- **Display OLED SSD1306 (128x64, I2C)**
- **Botões de entrada (Botão A e Botão B, Joystick)**
- **Buzzer piezoelétrico**

## Bibliotecas Utilizadas
- `pico/stdlib.h` - Funções padrão da Raspberry Pi Pico
- `hardware/i2c.h` - Controle do display OLED via I2C
- `hardware/pio.h` - Controle da matriz de LEDs WS2812
- `ssd1306.h` - Biblioteca para controle do display OLED
- `time.h` - Geração de números aleatórios e delays

## Como Jogar
1. Pressione o **botão do joystick** para iniciar o jogo.
2. Um padrão de LED acenderá na matriz.
3. Pressione **Botão A** se o LED estiver na posição esquerda.
4. Pressione **Botão B** se o LED estiver na posição direita.
5. Se acertar, ganha pontos e o jogo acelera.
6. Se errar, o jogo termina e o recorde é atualizado.
7. Pressione o **botão do joystick** para reiniciar.

## Link do Vídeo
O vídeo de apresentação pode ser acessado [aqui](https://drive.google.com/file/d/1lI5Ah1_1KuR9Q9kWEay5FCQXMxBB1vG0/view?usp=sharing).
