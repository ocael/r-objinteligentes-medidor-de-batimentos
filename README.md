# Sistema de Monitoramento Cardíaco com ESP32

## Descrição do Projeto

Este projeto consiste em um sistema de monitoramento cardíaco em tempo real, utilizando um sensor de pulso, display OLED, LEDs indicadores e um buzzer, integrado a um microcontrolador ESP32. O objetivo é monitorar a frequência cardíaca (BPM) e informar se o ritmo do usuário está baixo, normal ou elevado, além de enviar os dados via MQTT para monitoramento remoto.

O sistema realiza leituras periódicas do sensor de pulso e apresenta os valores no display OLED, aciona LEDs indicadores e emite alertas sonoros quando o BPM está fora da faixa considerada ideal.

---

## Componentes Principais

- **ESP32 DevKit-C V4**: microcontrolador responsável por processar os dados do sensor, controlar os atuadores e gerenciar a comunicação via Wi-Fi e MQTT.  
- **Pulse Sensor (sensor de pulso)**: detecta os batimentos cardíacos do usuário e envia sinais analógicos para o ESP32.  
- **Display OLED 128x64**: exibe mensagens informando o estado do ritmo cardíaco e o BPM atual.  
- **LEDs indicadores**:  
  - **LED Vermelho**: indica BPM acima do limite.  
  - **LED Verde**: indica BPM normal.  
  - **LED Amarelo**: indica BPM abaixo do limite.  
- **Buzzer**: emite alertas sonoros quando o ritmo cardíaco está fora da faixa normal.  
- **Rede Wi-Fi**: conexão utilizada para enviar os dados para um broker MQTT remoto.  
- **Broker MQTT**: servidor utilizado para integração IoT e monitoramento remoto (`test.mosquitto.org`).

---

## Especificações Técnicas

- **Faixa de BPM monitorada**: 50 a 120 BPM  
- **Display OLED**: 128x64 pixels, interface I2C  
- **Conexão Wi-Fi**: 2.4 GHz, ESP32 integrado  
- **Protocolo de comunicação**: MQTT  
- **LEDs e buzzer**: acionamento digital direto pelos pinos do ESP32  
- **Pinos utilizados**:  
  - Sensor de pulso: GPIO 35  
  - LED baixo: GPIO 2  
  - LED normal: GPIO 4  
  - LED alto: GPIO 5  
  - Buzzer: GPIO 25  

---

## Funcionalidades

1. **Leitura do sensor de pulso**: o ESP32 captura sinais analógicos do sensor e converte em BPM.  
2. **Classificação do ritmo cardíaco**:  
   - BPM ≤ 50 → ritmo abaixo do ideal (LED amarelo + buzzer).  
   - BPM 51–119 → ritmo estável (LED verde).  
   - BPM ≥ 120 → ritmo elevado (LED vermelho + buzzer).  
3. **Exibição em tempo real**: o display OLED mostra o BPM e a categoria do ritmo cardíaco.  
4. **Alertas sonoros**: buzzer emite sinais quando o BPM está fora da faixa ideal.  
5. **Publicação via MQTT**: envia o BPM e estado do ritmo para tópicos remotos (`monitor/cardiaco/valor` e `monitor/cardiaco/estado`).  
6. **Monitoramento remoto**: qualquer dispositivo conectado ao broker MQTT pode receber os dados atualizados em tempo real.

---

## Instalação e Configuração

1. Instale a IDE Arduino e adicione o suporte ao ESP32.  
2. Conecte o ESP32 ao computador via cabo USB.  
3. Instale as bibliotecas necessárias:  
   - `Adafruit_GFX`  
   - `Adafruit_SSD1306`  
   - `PubSubClient`  
4. Configure os parâmetros de Wi-Fi e MQTT no código:  

```cpp
const char* wifiSSID = "SEU_SSID";
const char* wifiPASS = "SUA_SENHA";
const char* brokerMQTT = "test.mosquitto.org";
```

5. Faça o upload do código para o ESP32.  
6. Abra o monitor serial para acompanhar a conexão e os dados do sensor.

---

## Testes Realizados

- **Leitura do sensor**: simulações com diferentes sinais analógicos mostraram que o ESP32 interpreta corretamente o BPM.  
- **Acionamento dos LEDs**: cada faixa de BPM aciona o LED correspondente e, quando necessário, o buzzer.  
- **Display OLED**: apresenta os valores de BPM e o estado do ritmo em tempo real.  
- **MQTT**: o ESP32 conecta-se ao broker e publica corretamente os tópicos, permitindo monitoramento remoto.  

**Resultados obtidos**: sistema estável, leituras precisas e alertas funcionando de acordo com a faixa de BPM configurada.

---

## Possíveis Melhorias

- Implementar armazenamento de histórico de BPM em banco de dados.  
- Desenvolver dashboard web ou app para visualização gráfica do ritmo cardíaco.  
- Adicionar notificações em smartphone quando BPM estiver fora da faixa.  
- Implementar múltiplos sensores para monitoramento simultâneo de várias pessoas.

---

## Referências

- Adafruit SSD1306 OLED Library Documentation  
- PubSubClient MQTT Library for Arduino  
- ESP32 DevKit-C V4 Technical Reference  
- HiveMQ MQTT Broker Documentation
