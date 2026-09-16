#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClientSecure.h>
#include <ArduinoJson.h>

// ==========================
// WIFI
// ==========================
const char* ssid = "S24";
const char* password = "11111111";

// ==========================
// HTTP SERVER
// ==========================
const char* serverUrl = "https://httpbin.org/post";

// ==========================
// SETUP
// ==========================
void setup() {

  Serial.begin(115200);

  // Menghubungkan ke WiFi
  WiFi.begin(ssid, password);

  Serial.print("Menghubungkan ke WiFi");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println();
  Serial.println("WiFi berhasil terhubung!");

  Serial.print("IP ESP8266: ");
  Serial.println(WiFi.localIP());
}

// ==========================
// LOOP
// ==========================
void loop() {

  if (WiFi.status() == WL_CONNECTED) {

    // Client HTTPS
    WiFiClientSecure client;

    // Untuk pengujian
    // tidak melakukan verifikasi sertifikat
    client.setInsecure();

    HTTPClient http;

    // Memulai koneksi ke server
    http.begin(client, serverUrl);

    // Header JSON
    http.addHeader("Content-Type", "application/json");

    // ==========================
    // MEMBUAT DATA JSON
    // ==========================
    JsonDocument doc;

    doc["suhu"] = 28.5;
    doc["kelembaban"] = 65.0;

    // Mengubah JSON menjadi String
    String requestBody;

    serializeJson(doc, requestBody);

    // Menampilkan data yang akan dikirim
    Serial.print("Mengirim data: ");
    Serial.println(requestBody);

    // ==========================
    // HTTP POST
    // ==========================
    int httpResponseCode = http.POST(requestBody);

    // ==========================
    // CEK RESPONSE
    // ==========================
    if (httpResponseCode > 0) {

      Serial.print("Kode Response HTTP: ");
      Serial.println(httpResponseCode);

      Serial.println("Isi Response:");
      Serial.println(http.getString());

    } else {

      Serial.print("Pengiriman gagal, kode error: ");
      Serial.println(httpResponseCode);
    }

    // Mengakhiri koneksi
    http.end();
  }

  // Kirim setiap 10 detik
  delay(10000);
}
