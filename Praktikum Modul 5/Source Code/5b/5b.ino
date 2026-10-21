#if defined(ESP8266)
  #include <ESP8266WiFi.h>
#elif defined(ESP32)
  #include <WiFi.h>
#endif

#include <PubSubClient.h>
#include <ArduinoJson.h>

const char* ssid = "Authentic Nasgor Tuna Asap";
const char* password = "";

const char* mqtt_server = "broker.hivemq.com";
const int mqtt_port = 1883;
const char* topicData = "unsoed/tk245004/kelompokdupan/data"; // Sesuaikan nama kelompok

WiFiClient espClient;
PubSubClient client(espClient);

unsigned long lastMsg = 0;

// Mock function pengganti sensor DHT22
float bacaSuhuMock() {
    static unsigned long jumlahBaca = 0;
    jumlahBaca++;

    // Suhu dasar berubah secara bertahap
    float suhuDasar = 28.0 + 2.0 * sin(jumlahBaca * 0.08);

    // Noise kecil untuk mensimulasikan fluktuasi sensor
    float noise = random(-5, 6) * 0.1;

    float suhu = suhuDasar + noise;

    // Lonjakan suhu setiap 25 pembacaan untuk simulasi anomali
    if (jumlahBaca % 25 == 0) {
        suhu += 8.0;
    }

    return suhu;
}

void setup_wifi() {
  delay(10);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
}

void reconnect() {
  while (!client.connected()) {
    String clientId = "ESPClient-";
    clientId += String(random(0xffff), HEX);
    if (client.connect(clientId.c_str())) {
      // Terhubung
    } else {
      delay(5000);
    }
  }
}

void setup() {
  Serial.begin(115200);
  randomSeed(analogRead(0));
  setup_wifi();
  client.setServer(mqtt_server, mqtt_port);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  unsigned long now = millis();
  if (now - lastMsg > 5000) { // Kirim data setiap 5 detik
    lastMsg = now;

    float suhu = bacaSuhuMock();

    StaticJsonDocument<128> doc;
    doc["suhu"] = suhu;

    char buffer[128];
    serializeJson(doc, buffer);

    client.publish(topicData, buffer);
    Serial.print("Data Terkirim: ");
    Serial.println(buffer);
  }
}
