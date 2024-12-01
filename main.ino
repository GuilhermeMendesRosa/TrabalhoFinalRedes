#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <HTTPClient.h>
#include <PubSubClient.h>

// Credenciais da rede Wi-Fi
const char* ssid = "iPhone de Guilherme";
const char* password = "guilherme123";

// Configurações do Broker MQTT no HiveMQ Cloud
const char* mqttServer = "eebb436e4f284be292dfca48e94194fc.s1.eu.hivemq.cloud";
const int mqttPort = 8883;
const char* mqttUsername = "admin";
const char* mqttPassword = "JeK5NWiDe@K@dfY";
const char* mqttTopic = "leonunes/waterlevel";
const char* alertTopic = "leonunes/alert";
const char* infoTopic = "leonunes/info";

// Configurações do Telegram
const char* telegramBotToken = "7701872330:AAHpcTxKjSwM2Ll9RmmdQT7SyPxgIJ9D8n4";  // Substitua pelo token do bot
const char* telegramChatId = "5438697248";      // Substitua pelo ID do chat

WiFiClientSecure espClient; 
PubSubClient client(espClient);

#define BOIA 19  // Pino do sensor de boia

void setupWiFi() {
  WiFi.begin(ssid, password);
  Serial.print("Conectando ao WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConectado ao WiFi com sucesso.");
}

void connectToMQTT() {
  while (!client.connected()) {
    Serial.println("Conectando ao Broker MQTT...");
    if (client.connect("ESP32Client", mqttUsername, mqttPassword)) {
      Serial.println("Conectado ao Broker MQTT!");
    } else {
      Serial.print("Falha na conexão, código de erro = ");
      Serial.println(client.state());
      delay(2000);
    }
  }
}

// Função para enviar mensagem ao Telegram
void sendTelegramMessage(const char* message) {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient https;
    String url = String("https://api.telegram.org/bot") + telegramBotToken + "/sendMessage";
    url += "?chat_id=" + String(telegramChatId);
    url += "&text=" + String(message);

    https.begin(url);
    int httpCode = https.GET();

    if (httpCode > 0) {
      Serial.printf("Mensagem enviada ao Telegram, código: %d\n", httpCode);
    } else {
      Serial.printf("Erro ao enviar mensagem ao Telegram: %s\n", https.errorToString(httpCode).c_str());
    }
    https.end();
  } else {
    Serial.println("WiFi desconectado, não foi possível enviar mensagem ao Telegram.");
  }
}

void setup() {
  Serial.begin(115200);
  pinMode(BOIA, INPUT);
  setupWiFi();

  espClient.setInsecure();

  client.setServer(mqttServer, mqttPort);
  connectToMQTT();
}

void loop() {
  if (!client.connected()) {
    connectToMQTT();
  }

  client.loop();

  int cheio = digitalRead(BOIA);
  unsigned long currentMillis = millis();
  char message[100];

  if (cheio != LOW) {  // Alerta: Nível baixo
    snprintf(message, 100, "ALERTA: Nível de água baixo. Timestamp: %lu ms", currentMillis);
    Serial.println(message);
    client.publish(mqttTopic, message);
    client.publish(alertTopic, "ALERTA: O nível de água está baixo, verifique o sistema.");
    sendTelegramMessage("⚠️ ALERTA: O nível de água está baixo! Verifique o sistema imediatamente.");
  } else {  // Nível normal
    snprintf(message, 100, "Nível de água cheio. Timestamp: %lu ms", currentMillis);
    Serial.println(message);
    client.publish(mqttTopic, message);
  }

  static unsigned long lastInfoPublish = 0;
  if (millis() - lastInfoPublish > 10000) {
    lastInfoPublish = millis();
    snprintf(message, 100, "IP ESP32: %s, Porta MQTT = %d, Timestamp: %lu ms", WiFi.localIP().toString().c_str(), mqttPort, currentMillis);
    client.publish(infoTopic, message);
  }

  delay(5000);
}