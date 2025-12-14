Visão geral do projeto

O projeto é uma estação barométrica baseada no ATmega328P que lê:

Pressão atmosférica (hPa) e temperatura (°C) do BMP180 via I²C (TWI)

Exibe os dados em um LCD alfanumérico (20x4/16x2) usando um expansor I/O PCF8574 (LCD em modo 4 bits com apenas 2 fios no barramento)

Possui LEDs de status (ex.: atividade, leitura, erro)

Usa resistores pull-up no barramento I²C para estabilizar SDA/SCL

Principais blocos do hardware
1) Microcontrolador (ATmega328P)

É o “cérebro” do sistema.

Executa:

inicialização do I²C

leitura do BMP180

cálculos de compensação (temperatura e pressão reais)

controle do display LCD via PCF8574

lógica de atualização periódica

2) Sensor BMP180 (I²C)

Trabalha no endereço 0x77 (7-bit).

Fornece:

UT: temperatura bruta (uncompensated temperature)

UP: pressão bruta (uncompensated pressure)

O sensor tem coeficientes de calibração internos gravados na sua PROM:

AC1, AC2, AC3, AC4, AC5, AC6, B1, B2, MB, MC, MD

Esses coeficientes são lidos de 0xAA até 0xBF e usados nas fórmulas do datasheet para compensar as leituras.

3) Barramento I²C compartilhado

No teu esquema, BMP180 e PCF8574 estão no mesmo barramento:

SDA e SCL do ATmega328P (PC4/PC5)

R1 e R2 = 4k7 (pull-ups)

Esse barramento permite controlar:

sensor (leitura de pressão/temperatura)

display (via PCF8574)

4) PCF8574 (I²C → paralela do LCD)

Recebe comandos I²C e “traduz” para os pinos do LCD:

dados (D4..D7)

controle (RS, E, Backlight)

Isso reduz muito o número de pinos necessários.

5) LCD (LM044L no Proteus / LCD padrão HD44780)

Exibe:

pressão (hPa)

temperatura (°C)

(pode exibir também menus: altitude, previsão, calendário etc.)

6) Ajuste de contraste do LCD (RV1)

Potenciômetro ligado ao pino V0/VEE do LCD para ajustar contraste.

7) LEDs (status)

Diodos LED (vermelho/verde) usados para:

indicar vida do sistema

indicar leitura I²C

indicar erro (ex.: falha no sensor)

Funcionamento lógico do software
Etapa A — Inicialização

Configura clock e periféricos

Inicializa TWI/I²C

Inicializa LCD

Lê coeficientes do BMP180 (AC1..MD)

Se falhar: mostra erro no LCD e/ou serial

Etapa B — Aquisição de dados

A cada ciclo:

Lê UT

escreve 0x2E em 0xF4

espera 4,5ms

lê 0xF6/0xF7

Lê UP

escreve 0x34+(oss<<6) em 0xF4

espera o tempo do OSS

lê 0xF6/0xF7/0xF8

Calcula:

Temperatura real (°C) usando UT + calibração

Pressão real (Pa/hPa) usando UP + calibração + oss

Etapa C — Exibição

Atualiza LCD com:

Pressão em hPa (ex.: 1002.7)

Temperatura em °C (ex.: 25.3)

LEDs podem piscar durante leitura/atualização

Por que ler AC1..MD é essencial?

Porque UT e UP são valores crus, e o BMP180 só fornece valores corretos depois que você aplica as fórmulas com os coeficientes internos.

Ou seja:

Sem AC1..MD, você até lê UT/UP

mas não consegue compensar e obter temperatura/pressão reais com precisão.
