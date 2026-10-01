#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>

// ==================================================
// WIFI
// ==================================================
const char* ssid = "Authentic Nasgor Tuna Asap";
const char* password = "12345678";

// ==================================================
// MQTT
// ==================================================
const char* mqttServer = "broker.hivemq.com";
const int mqttPort = 1883;

// Topic untuk LED
const char* topicLED =
    "unsoed/tk245004/viodupan/led";

// Topic baru untuk buzzer
const char* topicBuzzer =
    "unsoed/tk245004/viodupan/buzzer";

// Topic untuk mengirim data
const char* topicData =
    "unsoed/tk245004/viodupan/data";

// ==================================================
// AKTUATOR
// ==================================================

// LED menggunakan GPIO 5
const int ledPin = 5;

// Buzzer menggunakan GPIO 4
const int buzzerPin = 4;

// ==================================================
// MQTT CLIENT
// ==================================================
WiFiClient espClient;
PubSubClient client(espClient);

// ==================================================
// TIMER PUBLISH
// ==================================================
unsigned long waktuTerakhirPublish = 0;
const unsigned long intervalPublish = 5000;


// ==================================================
// CALLBACK MQTT
// Dipanggil ketika pesan masuk
// ==================================================
void callback(char* topic, byte* payload, unsigned int length) {

  String pesan;

  // Mengubah payload menjadi String
  for (unsigned int i = 0; i < length; i++) {
    pesan += (char)payload[i];
  }

  Serial.println();
  Serial.println("===== PESAN MASUK =====");

  Serial.print("Topic   : ");
  Serial.println(topic);

  Serial.print("Payload : ");
  Serial.println(pesan);


  // ==================================================
  // DESERIALISASI JSON
  // ==================================================
  JsonDocument doc;

  DeserializationError error =
      deserializeJson(doc, pesan);

  if (error) {

    Serial.print("Gagal parsing JSON: ");
    Serial.println(error.c_str());

    return;
  }


  // Mengambil perintah dari JSON
  const char* perintah = doc["perintah"];


  // ==================================================
  // MEMBEDAKAN TOPIC
  // ==================================================

  // Jika pesan berasal dari topic LED
  if (strcmp(topic, topicLED) == 0) {

    if (String(perintah) == "ON") {

      digitalWrite(ledPin, HIGH);
      Serial.println("LED: ON");

    }
    else if (String(perintah) == "OFF") {

      digitalWrite(ledPin, LOW);
      Serial.println("LED: OFF");
    }
  }


  // Jika pesan berasal dari topic BUZZER
  else if (strcmp(topic, topicBuzzer) == 0) {

    if (String(perintah) == "ON") {

      digitalWrite(buzzerPin, HIGH);
      Serial.println("BUZZER: ON");

    }
    else if (String(perintah) == "OFF") {

      digitalWrite(buzzerPin, LOW);
      Serial.println("BUZZER: OFF");
    }
  }

  Serial.println("======================");
}


// ==================================================
// WIFI
// ==================================================
void hubungkanWiFi() {

  WiFi.begin(ssid, password);

  Serial.print("Menghubungkan ke WiFi");

  while (WiFi.status() != WL_CONNECTED) {

    delay(500);
    Serial.print(".");
  }

  Serial.println();
  Serial.println("WiFi berhasil terhubung!");

  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
}


// ==================================================
// MQTT
// ==================================================
void hubungkanMQTT() {

  while (!client.connected()) {

    Serial.print("Menghubungkan ke broker MQTT...");

    String clientId =
        "ESP8266Client-" +
        String(random(0xffff), HEX);

    if (client.connect(clientId.c_str())) {

      Serial.println("berhasil terhubung!");

      // Subscribe topic LED
      client.subscribe(topicLED);

      // Subscribe topic Buzzer
      client.subscribe(topicBuzzer);

      Serial.println("Subscribe berhasil:");

      Serial.println(topicLED);
      Serial.println(topicBuzzer);
    }

    else {

      Serial.print("gagal, rc=");
      Serial.print(client.state());

      Serial.println(" coba lagi dalam 2 detik");

      delay(2000);
    }
  }
}


// ==================================================
// PUBLISH DATA
// ==================================================
void publishData() {

  JsonDocument doc;

  doc["device"] = "ESP8266";
  doc["status"] = "ONLINE";

  String jsonData;

  serializeJson(doc, jsonData);

  client.publish(topicData, jsonData.c_str());

  Serial.println();
  Serial.println("===== PUBLISH DATA =====");

  Serial.print("Topic   : ");
  Serial.println(topicData);

  Serial.print("Payload : ");
  Serial.println(jsonData);

  Serial.println("========================");
}


// ==================================================
// SETUP
// ==================================================
void setup() {

  Serial.begin(115200);

  // Mengatur LED sebagai OUTPUT
  pinMode(ledPin, OUTPUT);

  // Mengatur buzzer sebagai OUTPUT
  pinMode(buzzerPin, OUTPUT);

  // Kondisi awal LED mati
  digitalWrite(ledPin, LOW);

  // Kondisi awal buzzer mati
  digitalWrite(buzzerPin, LOW);

  // Hubungkan WiFi
  hubungkanWiFi();

  // Mengatur MQTT broker
  client.setServer(mqttServer, mqttPort);

  // Mendaftarkan callback
  client.setCallback(callback);
}


// ==================================================
// LOOP
// ==================================================
void loop() {

  // Jika MQTT terputus
  if (!client.connected()) {

    hubungkanMQTT();
  }

  // Memproses pesan MQTT
  client.loop();


  // ==================================================
  // PUBLISH MENGGUNAKAN MILLIS
  // ==================================================

  unsigned long waktuSekarang = millis();

  if (waktuSekarang - waktuTerakhirPublish >=
      intervalPublish) {

    waktuTerakhirPublish = waktuSekarang;

    publishData();
  }
}
