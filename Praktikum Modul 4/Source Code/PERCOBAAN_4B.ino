#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <DHT.h>

// ==================================================
// WIFI
// ==================================================
const char* ssid = "S24";
const char* password = "11111111";

// ==================================================
// MQTT
// ==================================================
const char* mqttServer = "broker.hivemq.com";
const int mqttPort = 1883;

const char* topicData =
    "unsoed/tk245004/viodupan/data";

const char* topicPerintah =
    "unsoed/tk245004/viodupan/perintah";

// ==================================================
// SENSOR DHT11
// ESP8266: D2 = GPIO4
// ==================================================
#define DHTPIN 4
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);

// ==================================================
// LED / AKTUATOR
// ESP8266: gpio 5
// ==================================================
const int ledPin = 5;

// ==================================================
// MQTT CLIENT
// ==================================================
WiFiClient espClient;
PubSubClient client(espClient);

// ==================================================
// TIMER PUBLISH
// ==================================================
unsigned long waktuTerakhirPublish = 0;

const long intervalPublish = 5000;
// Publish data setiap 5 detik


// ==================================================
// CALLBACK MQTT
// Dipanggil ketika pesan dari topic perintah diterima
// ==================================================
void callback(char* topic, byte* payload, unsigned int length) {

  String pesan;

  // Mengubah payload MQTT menjadi String
  for (unsigned int i = 0; i < length; i++) {
    pesan += (char)payload[i];
  }

  // Menampilkan pesan yang diterima
  Serial.println();
  Serial.println("===== PESAN PERINTAH =====");

  Serial.print("Topic   : ");
  Serial.println(topic);

  Serial.print("Pesan   : ");
  Serial.println(pesan);

  // ==================================================
  // DESERIALISASI JSON
  // ==================================================
  JsonDocument doc;

  DeserializationError error =
      deserializeJson(doc, pesan);

  if (error) {

    Serial.print("Parsing : GAGAL - ");
    Serial.println(error.c_str());

    return;
  }

  // Mengambil nilai "perintah"
  const char* perintah = doc["perintah"];

  // ==================================================
  // KONTROL LED
  // ==================================================
  if (String(perintah) == "ON") {

    digitalWrite(ledPin, HIGH);
    Serial.println("Aktuator: LED ON");

  }
  else if (String(perintah) == "OFF") {

    digitalWrite(ledPin, LOW);
    Serial.println("Aktuator: LED OFF");

  }
  else {

    Serial.println("Aktuator: Perintah tidak dikenali");
  }

  Serial.println("==========================");
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

    Serial.print("Menghubungkan ke MQTT... ");

    // Membuat Client ID secara acak
    String clientId =
        "ESP8266Client-" +
        String(random(0xffff), HEX);

    // Mencoba terhubung ke broker
    if (client.connect(clientId.c_str())) {

      Serial.println("berhasil!");

      // Subscribe topic perintah
      if (client.subscribe(topicPerintah)) {

        Serial.print("Subscribe berhasil: ");
        Serial.println(topicPerintah);

      }
      else {

        Serial.println("Subscribe gagal!");
      }

    }
    else {

      Serial.print("gagal, rc=");
      Serial.println(client.state());

      Serial.println("Mencoba kembali dalam 2 detik...");
      delay(2000);
    }
  }
}


// ==================================================
// SETUP
// ==================================================
void setup() {

  Serial.begin(115200);

  // Mengatur LED sebagai OUTPUT
  pinMode(ledPin, OUTPUT);

  // Kondisi awal LED mati
  digitalWrite(ledPin, LOW);

  // Memulai sensor DHT11
  dht.begin();

  // Menghubungkan ke WiFi
  hubungkanWiFi();

  // Mengatur MQTT broker
  client.setServer(mqttServer, mqttPort);

  // Mendaftarkan callback MQTT
  client.setCallback(callback);
}


// ==================================================
// LOOP
// ==================================================
void loop() {

  // ==================================================
  // CEK KONEKSI MQTT
  // ==================================================
  if (!client.connected()) {

    hubungkanMQTT();
  }

  // Memproses pesan MQTT yang masuk
  client.loop();


  // ==================================================
  // PUBLISH DATA SENSOR SETIAP 5 DETIK
  // ==================================================
  if (millis() - waktuTerakhirPublish >= intervalPublish) {

    waktuTerakhirPublish = millis();

    // Membaca suhu dari DHT11
    float suhu = dht.readTemperature();

    // Mengecek apakah pembacaan berhasil
    if (isnan(suhu)) {

      Serial.println("Gagal membaca sensor DHT11!");
      return;
    }

    // ==================================================
    // MEMBUAT DATA JSON
    // ==================================================
    JsonDocument doc;

    doc["suhu"] = suhu;

    char buffer[128];

    serializeJson(doc, buffer);

    // ==================================================
    // PUBLISH KE MQTT
    // ==================================================
    if (client.publish(topicData, buffer)) {

      Serial.print("Data terkirim: ");
      Serial.println(buffer);

    }
    else {

      Serial.println("Gagal mengirim data!");
    }
  }
}
