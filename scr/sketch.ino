
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <DHT.h>

// ==================== Definições de Pinos ====================

#define PIR_PIN 4
#define DHT_PIN 5
#define DHT_TYPE DHT22
#define LDR_PIN 34
#define LED_ILUM 25
#define LED_VENT 26
#define LED_STATUS 27
#define BUZZER_PIN 14
#define BTN_ILUM 32
#define BTN_MODO 33

// ==================== LCD I2C ====================

#define LCD_ADDR 0x27
#define LCD_COLS 16
#define LCD_ROWS 2

// ==================== Parâmetros do Sistema ====================

const int LIMITE_LUZ_BAIXA = 2000;
const int LIMITE_TEMP_ALTA = 28;
const int LIMITE_TEMP_BAIXA = 25;

const unsigned long TEMPO_SEM_PRESENCA = 10000UL;

// ==================== Objetos ====================

LiquidCrystal_I2C lcd(LCD_ADDR, LCD_COLS, LCD_ROWS);
DHT dht(DHT_PIN, DHT_TYPE);

// ==================== Variáveis Globais ====================

bool iluminacaoLigada = false;
bool ventoinhaLigada = false;
bool presencaRecente = false;

bool ultimoBtnIlum = HIGH;
bool ultimoBtnModo = HIGH;

bool bipPresenca = false;

int telaAtual = 0;

unsigned long ultimaLeitura = 0;
unsigned long ultimoMovimento = 0;

float temperatura = 0.0;
float umidade = 0.0;
int luminosidade = 0;

// ==================== Setup ====================

void setup() {

  Serial.begin(115200);

  pinMode(PIR_PIN, INPUT);
  pinMode(LDR_PIN, INPUT);
  pinMode(LED_ILUM, OUTPUT);
  pinMode(LED_VENT, OUTPUT);
  pinMode(LED_STATUS, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);

  pinMode(BTN_ILUM, INPUT_PULLUP);
  pinMode(BTN_MODO, INPUT_PULLUP);

  digitalWrite(LED_STATUS, HIGH);

  dht.begin();

  // Inicializa LCD I2C

  lcd.init();
  lcd.backlight();

  lcd.setCursor(0, 0);
  lcd.print("Automacao Resid.");

  lcd.setCursor(0, 1);
  lcd.print("Iniciando...");

  delay(1500);

  lcd.clear();
}

// ==================== Loop ====================

void loop() {

  unsigned long agora = millis();

  // -------- Leitura de sensores a cada 2 segundos --------

  if (agora - ultimaLeitura > 2000) {

    ultimaLeitura = agora;

    temperatura = dht.readTemperature();
    umidade = dht.readHumidity();
    luminosidade = analogRead(LDR_PIN);
  }

  // -------- Detecção de presença --------

  bool presenca = digitalRead(PIR_PIN);

  if (presenca) {

    ultimoMovimento = agora;

    if (!bipPresenca) {

      tone(BUZZER_PIN, 1500, 50);

      bipPresenca = true;
    }

  } else {

    bipPresenca = false;
  }

  presencaRecente = (agora - ultimoMovimento) < TEMPO_SEM_PRESENCA;

  // -------- Controle automático da iluminação --------

  if (presencaRecente && luminosidade < LIMITE_LUZ_BAIXA) {

    iluminacaoLigada = true;

  } else if (!presencaRecente) {

    iluminacaoLigada = false;
  }

  // -------- Controle da ventoinha (com histerese) --------

  if (!isnan(temperatura)) {

    if (temperatura > LIMITE_TEMP_ALTA) {
      ventoinhaLigada = true;
    }

    if (temperatura < LIMITE_TEMP_BAIXA) {
      ventoinhaLigada = false;
    }
  }

  // -------- Botão 1 — Iluminação --------

  bool btnIlum = digitalRead(BTN_ILUM);

  if (ultimoBtnIlum == HIGH && btnIlum == LOW) {

    iluminacaoLigada = !iluminacaoLigada;
  }

  ultimoBtnIlum = btnIlum;

  // -------- Botão 2 — Alterna tela --------

  bool btnModo = digitalRead(BTN_MODO);

  if (ultimoBtnModo == HIGH && btnModo == LOW) {

    telaAtual = (telaAtual + 1) % 2;

    lcd.clear();
  }

  ultimoBtnModo = btnModo;

  // -------- Aplicar estados nos LEDs --------

  digitalWrite(LED_ILUM, iluminacaoLigada ? HIGH : LOW);

  digitalWrite(LED_VENT, ventoinhaLigada ? HIGH : LOW);

  // -------- Atualiza LCD --------

  atualizarLCD();

  delay(100);
}

// ==================== LCD ====================

void atualizarLCD() {

  static unsigned long ultimaAtualizacao = 0;

  if (millis() - ultimaAtualizacao < 500) return;

  ultimaAtualizacao = millis();

  if (telaAtual == 0) {

    // Tela 1 — Clima

    lcd.setCursor(0, 0);

    lcd.print("Temp:");

    if (isnan(temperatura)) {
      lcd.print("--.-");
    } else {
      lcd.print(temperatura, 1);
    }

    lcd.print("C   ");

    lcd.setCursor(0, 1);

    lcd.print("Umid:");

    if (isnan(umidade)) {
      lcd.print("--");
    } else {
      lcd.print(umidade, 0);
    }

    lcd.print("%    ");

  } else {

    // Tela 2 — Status

    lcd.setCursor(0, 0);

    lcd.print("Luz:");
    lcd.print(iluminacaoLigada ? "ON " : "OFF");

    lcd.print(" Vent:");
    lcd.print(ventoinhaLigada ? "ON " : "OFF");

    lcd.setCursor(0, 1);

    lcd.print("Pres:");
    lcd.print(presencaRecente ? "SIM" : "NAO");

    lcd.print("      ");
  }
}