# 🏠 Sistema de Automação Residencial com ESP32

Projeto desenvolvido para a disciplina de Sistemas Embarcados — IFPE Campus Garanhuns.
Sistema de automação residencial baseado em ESP32 que monitora sensores e controla
automaticamente lâmpada e ventoinha, com interface em LCD 16x2 I2C.

---

## 📋 Descrição do Funcionamento

O sistema realiza as seguintes funções:

1. **Detecção de presença (PIR):** identifica movimento no ambiente.
2. **Leitura de luminosidade (LDR):** verifica se o ambiente está escuro.
3. **Controle automático da iluminação:** liga a lâmpada quando há presença
   e o ambiente está escuro; desliga após 10s sem presença.
4. **Medição de temperatura e umidade (DHT22):** lê valores a cada 2 s.
5. **Controle automático da ventoinha:** liga quando T > 28 °C e desliga
   quando T < 25 °C (histerese).
6. **Exibição em LCD 16x2 I2C:** alterna entre tela de clima e tela de status.
7. **Controle manual por botões:**
   - Botão 1: liga/desliga a iluminação;
   - Botão 2: alterna a tela do LCD.
8. **Alarme sonoro (buzzer):** emite bipe curto ao detectar presença.
9. **Debug serial:** envia dados para o monitor serial a 115200 bps.

---

## 🔌 Pinagem do ESP32

| GPIO | Componente                       |
|------|----------------------------------|
| 4    | Sensor PIR (HC-SR501)            |
| 5    | Sensor DHT22                     |
| 34   | Sensor LDR (entrada analógica)   |
| 25   | Relé — Lâmpada                   |
| 26   | Relé — Ventoinha                 |
| 14   | Buzzer                           |
| 21   | LCD I2C — SDA                    |
| 22   | LCD I2C — SCL                    |
| 32   | Botão 1 (iluminação)             |
| 33   | Botão 2 (modo/tela)              |

---

## 🧩 Componentes Utilizados

| Componente                | Quantidade |
|---------------------------|------------|
| ESP32 DevKit C V4         | 1          |
| Sensor PIR HC-SE501       | 1          |
| Sensor DHT22              | 1          |
| LDR + resistor 10 kΩ      | 1          |
| Módulo relé 5 V (2 canais)| 1          |
| LCD 16x2 com módulo I2C   | 1          |
| Buzzer 5 V                | 1          |
| Botões push-button        | 2          |
| Resistores 220 Ω          | 2          |
| Protoboard + jumpers      | 1          |

---

## 🛠️ Bibliotecas Necessárias

- `Wire` (nativa do ESP32)
- `LiquidCrystal I2C`
- `DHT sensor library`

Instale pela IDE do Arduino:
**Sketch → Incluir Biblioteca → Gerenciar Bibliotecas**

Ou via PlatformIO, adicione ao `platformio.ini`:

```ini
lib_deps =
    marcoschwartz/LiquidCrystal_I2C@^1.1.4
    adafruit/DHT sensor library@^1.4.6
    adafruit/Adafruit Unified Sensor@^1.1.14
