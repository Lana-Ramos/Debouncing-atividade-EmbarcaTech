# Projeto: Controle de Matriz de LEDs WS2812 com Interrupções no RP2040

## Descrição
Este projeto implementa um sistema de controle para uma matriz 5x5 de LEDs endereçáveis WS2812 utilizando a placa de desenvolvimento BitDogLab e o microcontrolador RP2040. O sistema utiliza interrupções para lidar com os botões de entrada, garantindo uma interação eficiente e responsiva. O código também inclui tratamento de bouncing via software e controle de um LED RGB auxiliar.

## Objetivos
- Compreender o funcionamento e a aplicação de interrupções em microcontroladores.
- Implementar debouncing via software para corrigir o bouncing em botões.
- Manipular e controlar LEDs comuns e LEDs endereçáveis WS2812.
- Utilizar resistores de pull-up internos para os botões.
- Desenvolver um projeto funcional que combine hardware e software.

## Componentes Utilizados
- **Placa BitDogLab (RP2040)**
- **Matriz de LEDs WS2812 (5x5)** conectada à GPIO 7
- **LED RGB** com conexões nas GPIOs 11 (verde), 12 (azul) e 13 (vermelho)
- **Botão A** conectado à GPIO 5
- **Botão B** conectado à GPIO 6

## Funcionalidades Implementadas
1. O LED vermelho pisca continuamente 5 vezes por segundo.
2. O **Botão A** incrementa o número exibido na matriz de LEDs.
3. O **Botão B** decrementa o número exibido na matriz de LEDs.
4. A matriz WS2812 exibe os números de 0 a 9 em um formato fixo, semelhante a caracteres digitais.

## Requisitos Atendidos
- **Uso de interrupções**: Todos os eventos dos botões são gerenciados via interrupção (IRQ).
- **Debouncing via software**: Implementado com um tempo de espera de 300ms para evitar leituras erradas.
- **Controle de LEDs**: Tanto os LEDs WS2812 quanto o LED RGB são utilizados.
- **Organização e comentários**: O código está devidamente comentado para facilitar o entendimento.

## Estrutura do Código
O código é estruturado da seguinte forma:
- **Configuração inicial**: Inicialização da matriz de LEDs, configuração dos botões e definição de interrupções.
- **Funções de controle**:
  - `put_pixel()`: Envia os valores de cor para os LEDs WS2812.
  - `urgb_u32()`: Converte valores RGB para o formato correto dos LEDs.
  - `set_one_led()`: Exibe um número na matriz de LEDs.
  - `blink_red()`: Faz o LED vermelho piscar continuamente.
- **Interrupções**:
  - `gpio_irq_handler()`: Gerencia os botões com debouncing e altera o número exibido.
- **Loop principal**: Mantém o LED vermelho piscando e atualiza a matriz de LEDs com o número correspondente.

## Como Utilizar
1. Compile e grave o firmware na placa BitDogLab.
2. Conecte a matriz de LEDs WS2812, o LED RGB e os botões conforme o esquema descrito.
3. Pressione o **Botão A** para incrementar o número na matriz de LEDs.
4. Pressione o **Botão B** para decrementar o número na matriz de LEDs.
5. Observe que o LED vermelho pisca 5 vezes por segundo de forma contínua.

## Autor
Lana Ramos Gomes

