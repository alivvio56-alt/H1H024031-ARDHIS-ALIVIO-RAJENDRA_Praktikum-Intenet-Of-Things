#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>

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

const char* topicPerintah =
    "unsoed/tk245004/viodupan/perintah";

// ==================================================
// LED / AKTUATOR
// ESP8266 D1 = GPIO5
// ==================================================
const int ledPin = 5;

// ==================================================
// MQTT CLIENT
// ==================================================
WiFiClient espClient;
PubSubClient client(espClient);

// ==================================================
// CALLBACK MQTT
// Dipanggil ketika pesan MQTT diterima
// ==================================================
void callback(char* topic, byte* payload, unsigned int length) {

  String pesan;

  // Mengubah payload menjadi String
  for (unsigned int i = 0; i < length; i++) {
    pesan += (char)payload[i];
  }

  // Menampilkan pesan mentah
  Serial.println();
  Serial.println("===== PESAN MQTT DITERIMA =====");
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

  // Menampilkan hasil parsing
  Serial.println("Parsing : BERHASIL");
  Serial.print("Perintah: ");
  Serial.println(perintah);

  // ==================================================
  // KONTROL AKTUATOR
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

  Serial.println("===============================");
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

    Serial.print("Menghubungkan ke broker MQTT... ");

    String clientId =
        "ESP8266Client-" +
        String(random(0xffff), HEX);

    if (client.connect(clientId.c_str())) {

      Serial.println("berhasil!");

      // Subscribe ke topic perintah
      client.subscribe(topicPerintah);

      Serial.print("Subscribe ke topic: ");
      Serial.println(topicPerintah);

    }
    else {

      Serial.print("gagal, rc=");
      Serial.print(client.state());

      Serial.println(" | mencoba lagi...");
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

  // Menghubungkan ke WiFi
  hubungkanWiFi();

  // Mengatur MQTT broker dan port
  client.setServer(mqttServer, mqttPort);

  // Mendaftarkan fungsi callback
  client.setCallback(callback);

  // Menghubungkan ke MQTT
  hubungkanMQTT();
}

// ==================================================
// LOOP
// ==================================================
void loop() {

  // Jika MQTT terputus, hubungkan kembali
  if (!client.connected()) {

    hubungkanMQTT();
  }

  // Memproses pesan MQTT yang masuk
  client.loop();
}
