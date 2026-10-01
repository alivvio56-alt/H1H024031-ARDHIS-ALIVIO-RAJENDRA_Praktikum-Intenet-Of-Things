#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>

// ==========================
// WIFI
// ==========================
const char* ssid = "Authentic Nasgor Tuna Asap";
const char* password = "12345678";

// ==========================
// MQTT
// ==========================
const char* mqttServer = "broker.hivemq.com";
const int mqttPort = 1883;

const char* topicPerintah =
    "unsoed/tk245004/viodupan/perintah";

// ==========================
// LED / AKTUATOR
// ==========================
// GPIO 5 / D1
const int ledPin = 5;

// ==========================
// MQTT CLIENT
// ==========================
WiFiClient espClient;
PubSubClient client(espClient);


// ==================================================
// CALLBACK
// ==================================================
void callback(char* topic, byte* payload, unsigned int length) {

  String pesan;

  // Mengubah payload menjadi String
  for (unsigned int i = 0; i < length; i++) {
    pesan += (char)payload[i];
  }

  Serial.print("Pesan diterima [");
  Serial.print(topic);
  Serial.print("]: ");
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


  // ==================================================
  // MENGAMBIL DATA JSON
  // ==================================================

  const char* perintah = doc["perintah"];

  // Mengambil nilai intensitas dari JSON
  int intensitas = doc["intensitas"];


  // ==================================================
  // KONTROL LED DENGAN PWM
  // ==================================================

  if (String(perintah) == "ON") {

    analogWrite(ledPin, intensitas);

    Serial.print("Aktuator: ON | Intensitas: ");
    Serial.println(intensitas);

  }
  else if (String(perintah) == "OFF") {

    analogWrite(ledPin, 0);

    Serial.println("Aktuator: OFF");
  }
}


// ==================================================
// MENGHUBUNGKAN ESP8266 KE WIFI
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
// MENGHUBUNGKAN ESP8266 KE MQTT BROKER
// ==================================================
void hubungkanMQTT() {

  while (!client.connected()) {

    Serial.print("Menghubungkan ke broker MQTT...");

    String clientId =
        "ESP8266Client-" +
        String(random(0xffff), HEX);

    if (client.connect(clientId.c_str())) {

      Serial.println("berhasil terhubung!");

      client.subscribe(topicPerintah);

      Serial.print("Subscribe ke topic: ");
      Serial.println(topicPerintah);

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
// SETUP
// ==================================================
void setup() {

  Serial.begin(115200);

  pinMode(ledPin, OUTPUT);

  // Kondisi awal LED mati
  analogWrite(ledPin, 0);

  hubungkanWiFi();

  client.setServer(mqttServer, mqttPort);

  client.setCallback(callback);
}


// ==================================================
// LOOP
// ==================================================
void loop() {

  if (!client.connected()) {
    hubungkanMQTT();
  }

  client.loop();
}
