# Projeto de Comunicação Serial

Este projeto implementa um sistema de comunicação serial utilizando o Raspberry Pi Pico W, integrando um display OLED, uma matriz de LEDs WS2812 e botões para interação.
Link para vídeo demonstrativo do projeto: https://drive.google.com/file/d/1c9utOsgYzQM17zIQ1Rc8hG5jpVC6waa9/view?usp=sharing

## 📌 Funcionalidades
- Exibição de caracteres no display OLED
- Controle de uma matriz de LEDs WS2812
- Interação via botões para alternar estados dos LEDs
- Comunicação serial via USB

## 🔧 Requisitos de Hardware
- Raspberry Pi Pico W
- Display OLED SSD1306
- Matriz de LEDs WS2812 (5x5)
- 2 botões de pressão
- 2 LEDs (verde e azul)

## ⚙️ Configuração do Ambiente
1. Instale o SDK do Raspberry Pi Pico
2. Configure o ambiente de desenvolvimento (ex: VS Code com extensão Pico-W-Go)
3. Clone este repositório:

```bash
git clone https://github.com/biaslima/Tarefa_Comunicacao_Serial.git
cd Tarefa_Comunicacao_Serial
```

## 🚀 Compilação e Upload
```bash
mkdir build
cd build
cmake ..
make
```

Após a compilação, copie o arquivo `.uf2` gerado para o Raspberry Pi Pico W em modo de bootloader.

## 🛠️ Uso
1. Conecte o Pico W via USB
2. Abra um terminal serial (115200 baud)
3. Digite caracteres para exibir no OLED
4. Use os botões para controlar os LEDs
