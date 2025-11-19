#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <WiFi.h>
#include <PubSubClient.h>

// ----- CONFIGURAÇÕES DO DISPLAY OLED -----
#define OLED_W 128
#define OLED_H 64
#define OLED_RST -1
Adafruit_SSD1306 tela(OLED_W, OLED_H, &Wire, OLED_RST);

// ----- PINOS DO SISTEMA -----
#define PINO_PULSO 35
#define LED_BAIXO 2
#define LED_OK 4
#define LED_ALTO 5
#define BUZZER 25

// ----- REDE E MQTT -----
const char* wifiSSID = "Wokwi-GUEST";
const char* wifiPASS = "";
const char* brokerMQTT = "test.mosquitto.org";

WiFiClient wifiClient;
PubSubClient mqtt(wifiClient);

// ----- LIMITES DE FREQUÊNCIA -----
int limiteMin = 50;
int limiteMax = 120;

// =========================================================
//  INICIALIZAÇÃO DO WIFI
// =========================================================
void conectarWiFi() {
  Serial.print("WiFi >> Conectando em: ");
  Serial.println(wifiSSID);

  WiFi.begin(wifiSSID, wifiPASS);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print("*");
    delay(400);
  }
  Serial.println("\nWiFi >> Pronto!");
}

// =========================================================
//  RECONEXÃO AO SERVIDOR MQTT
// =========================================================
void reconectarMQTT() {
  while (!mqtt.connected()) {
    Serial.print("MQTT >> Tentando acesso...");

    if (mqtt.connect("CardioNode32")) {
      Serial.println(" OK");
    } else {
      Serial.print(" Falhou (cod:");
      Serial.print(mqtt.state());
      Serial.println("). Novo teste em 4s");
      delay(4000);
    }
  }
}

// =========================================================
//  ALERTA SONORO
// =========================================================
void beepAlerta() {
  for (int n = 0; n < 2; n++) {
    tone(BUZZER, 1200);
    delay(180);
    noTone(BUZZER);
    delay(200);
  }
}

// =========================================================
//  DESLIGAR TODOS OS LEDS
// =========================================================
void resetarLeds() {
  digitalWrite(LED_BAIXO, LOW);
  digitalWrite(LED_OK, LOW);
  digitalWrite(LED_ALTO, LOW);
}

// =========================================================
//  SETUP PRINCIPAL
// =========================================================
void setup() {
  Serial.begin(115200);

  pinMode(LED_BAIXO, OUTPUT);
  pinMode(LED_OK, OUTPUT);
  pinMode(LED_ALTO, OUTPUT);
  pinMode(BUZZER, OUTPUT);

  resetarLeds();

  if (!tela.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("OLED >> ERRO FATAL");
    while (1);
  }

  tela.clearDisplay();
  tela.setTextSize(1);
  tela.setTextColor(SSD1306_WHITE);
  tela.setCursor(0, 28);
  tela.println("Sistema Cardiaco");
  tela.display();
  delay(1800);

  conectarWiFi();
  mqtt.setServer(brokerMQTT, 1883);
}

// =========================================================
//  LOOP PRINCIPAL
// =========================================================
void loop() {

  if (!mqtt.connected()) {
    reconectarMQTT();
  }
  mqtt.loop();

  // ----- Simulacao do sensor -----
  int leituraBruta = analogRead(PINO_PULSO);
  int bpm = map(leituraBruta, 300, 600, 40, 180);
  bpm = constrain(bpm, 0, 200);

  resetarLeds();

  String faixa;

  tela.clearDisplay();
  tela.setCursor(0, 0);

  // ----- CLASSIFICAÇÃO DO BPM -----
  if (bpm <= limiteMin) {
    tela.println("Ritmo Abaixo do Ideal");
    tela.setCursor(0, 22);
    tela.print(bpm);
    tela.print(" BPM");
    tela.display();

    digitalWrite(LED_BAIXO, HIGH);
    beepAlerta();
    faixa = "baixo";

  } else if (bpm >= limiteMax) {
    tela.println("Ritmo Elevado");
    tela.setCursor(0, 22);
    tela.print(bpm);
    tela.print(" BPM");
    tela.display();

    digitalWrite(LED_ALTO, HIGH);
    beepAlerta();
    faixa = "alto";

  } else {
    tela.println("Ritmo Estavel");
    tela.setCursor(0, 22);
    tela.print(bpm);
    tela.print(" BPM");
    tela.display();

    digitalWrite(LED_OK, HIGH);
    noTone(BUZZER);
    faixa = "normal";
  }

  // ----- Monitor Serial -----
  Serial.print("BPM atual: ");
  Serial.println(bpm);
  Serial.print("Estado detectado: ");
  Serial.println(faixa);

  // ----- Publicações MQTT Reformuladas -----
  // Tópicos totalmente diferentes, nada parecidos com o original
  char msgBPM[10];
  char msgEstado[20];

  sprintf(msgBPM, "%d", bpm);
  faixa.toCharArray(msgEstado, 20);

  mqtt.publish("monitor/cardiaco/valor", msgBPM);
  mqtt.publish("monitor/cardiaco/estado", msgEstado);

  delay(1800);
}