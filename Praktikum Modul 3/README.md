# Praktikum IoT 3 — Protokol Komunikasi

Praktikum ini membahas implementasi komunikasi data pada sistem **Internet of Things (IoT)** menggunakan protokol **HTTP** dan **MQTT** dengan format pertukaran data **JSON**. Pengujian dilakukan menggunakan **ESP8266** yang terhubung ke jaringan WiFi.

## Tujuan

- Memahami konsep dasar protokol komunikasi pada IoT.
- Memahami perbedaan HTTP dan MQTT.
- Mengimplementasikan pengiriman data menggunakan HTTP POST.
- Mengimplementasikan komunikasi MQTT dengan pola publish-subscribe.
- Menggunakan JSON sebagai format pertukaran data.

## Perangkat dan Library

### Perangkat
- ESP8266
- Laptop/PC
- Kabel USB
- Jaringan WiFi
- MQTT Explorer

### Library
```cpp
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClientSecure.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
```
> Catatan: `ESP8266WiFi`, `ESP8266HTTPClient`, dan `WiFiClientSecure` tersedia melalui board package ESP8266. `PubSubClient` digunakan untuk MQTT dan `ArduinoJson` untuk pengolahan JSON.

---

## Percobaan 3A — HTTP POST

### Deskripsi

Pada percobaan 3A, ESP8266 mengirim data suhu dan kelembaban dalam format JSON ke server menggunakan metode **HTTP POST**. Endpoint pengujian yang digunakan adalah:

```text
https://httpbin.org/post
```

Hasil pengiriman diamati melalui **Serial Monitor** dengan melihat HTTP response code dan response body.

### Alur

```text
ESP8266
   ↓
WiFi
   ↓
HTTP POST + JSON
   ↓
httpbin.org/post
   ↓
HTTP Response
   ↓
Serial Monitor
```

### Data yang dikirim

```json
{
  "suhu": 28.5,
  "kelembaban": 65
}
```

### Hasil Pengujian

Contoh hasil yang diperoleh:

```text
Mengirim data: {"suhu":28.5,"kelembaban":65}
Kode Response HTTP: 200
Isi Response:
{
  ...
  "json": {
    "kelembaban": 65,
    "suhu": 28.5
  },
  ...
}
```

**Status pengiriman: Berhasil**

### Fungsi Penting

| Fungsi | Kegunaan |
|---|---|
| `WiFi.begin()` | Menghubungkan ESP8266 ke WiFi |
| `http.begin()` | Menentukan server/endpoint |
| `http.addHeader()` | Menentukan tipe data JSON |
| `serializeJson()` | Mengubah objek JSON menjadi teks |
| `http.POST()` | Mengirim data ke server |
| `http.getString()` | Membaca response body |
| `http.end()` | Mengakhiri koneksi HTTP |

---

## Percobaan 3B — MQTT

### Deskripsi

Pada percobaan 3B, ESP8266 mengirim data JSON menggunakan protokol **MQTT** dengan pola **publish-subscribe**. Broker yang digunakan adalah:

```text
broker.hivemq.com
Port: 1883
```

Data dipublikasikan ke topic:

```text
ardhis/esp8266/latihan
```

Data yang diterima diverifikasi menggunakan **MQTT Explorer** sebagai subscriber.

### Alur

```text
ESP8266 (Publisher)
        ↓
       WiFi
        ↓
MQTT Broker HiveMQ
        ↓
      Topic
        ↓
MQTT Explorer (Subscriber)
```

### Data yang dipublish

```json
{
  "suhu": 28.5,
  "kelembaban": 65
}
```

### Hasil Pengujian

Data berhasil diterima oleh MQTT Explorer secara berkala, dengan contoh output:

```text
{"suhu":28.5,"kelembaban":65}
```

### Fungsi Penting

| Fungsi | Kegunaan |
|---|---|
| `WiFi.begin()` | Menghubungkan ESP8266 ke WiFi |
| `client.setServer()` | Menentukan broker dan port MQTT |
| `client.connect()` | Menghubungkan ESP8266 ke broker |
| `client.connected()` | Mengecek koneksi MQTT |
| `client.loop()` | Menjaga komunikasi MQTT tetap berjalan |
| `serializeJson()` | Mengubah objek JSON menjadi teks |
| `client.publish()` | Mengirim data ke topic MQTT |

---

## Modifikasi Program — Menambahkan Waktu `millis()`

Program MQTT dimodifikasi agar JSON memiliki data tambahan berupa **waktu dalam milidetik sejak perangkat dinyalakan** menggunakan fungsi `millis()`.

```cpp
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>

// ==========================
// WIFI
// ==========================
const char* ssid = "s24";
const char* password = "11111111";

// ==========================
// MQTT
// ==========================
const char* mqttServer = "broker.hivemq.com";
const int mqttPort = 1883;
const char* mqttTopic = "ardhis/esp8266/latihan";

// ==========================
// OBJECT
// ==========================
WiFiClient espClient;
PubSubClient client(espClient);

// ==========================
// HUBUNGKAN WIFI
// ==========================
void hubungkanWiFi() {

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
// HUBUNGKAN MQTT
// ==========================
void hubungkanMQTT() {

  while (!client.connected()) {

    Serial.print("Menghubungkan ke broker MQTT...");

    String clientId = "ESP8266Client-";
    clientId += String(ESP.getChipId(), HEX);

    if (client.connect(clientId.c_str())) {

      Serial.println("berhasil terhubung!");

    } else {

      Serial.print("gagal, rc=");
      Serial.print(client.state());
      Serial.println(" coba lagi dalam 2 detik");

      delay(2000);
    }
  }
}

// ==========================
// SETUP
// ==========================
void setup() {

  Serial.begin(115200);

  hubungkanWiFi();

  client.setServer(mqttServer, mqttPort);
}

// ==========================
// LOOP
// ==========================
void loop() {

  if (!client.connected()) {
    hubungkanMQTT();
  }

  client.loop();

  // ==========================
  // MEMBUAT DATA JSON
  // ==========================
  JsonDocument doc;

  doc["suhu"] = 28.5;
  doc["kelembaban"] = 65.0;
  doc["waktu"] = millis();   // BARIS TAMBAHAN

  char buffer[160];

  serializeJson(doc, buffer);

  // ==========================
  // PUBLISH KE MQTT
  // ==========================
  bool berhasil = client.publish(mqttTopic, buffer);

  if (berhasil) {
    Serial.print("Data terkirim ke topic ");
    Serial.print(mqttTopic);
    Serial.print(": ");
    Serial.println(buffer);
  } else {
    Serial.println("Gagal mengirim data!");
  }

  delay(5000);
}
```

### Perubahan Kode

Sebelum modifikasi:

```cpp
doc["suhu"] = 28.5;
doc["kelembaban"] = 65.0;
```

Setelah modifikasi:

```cpp
doc["suhu"] = 28.5;
doc["kelembaban"] = 65.0;
doc["waktu"] = millis();
```

Ukuran buffer juga diperbesar:

```cpp
char buffer[160];
```

### Penjelasan

- `doc["waktu"]` menambahkan field baru bernama `waktu`.
- `millis()` menghasilkan waktu yang telah berlalu dalam satuan milidetik sejak program mulai berjalan.
- Buffer diperbesar agar cukup menampung JSON yang sekarang memiliki field tambahan.

### Contoh Hasil

```json
{
  "suhu": 28.5,
  "kelembaban": 65,
  "waktu": 15234
}
```

Nilai `15234` menunjukkan program telah berjalan sekitar **15,234 ms** sejak perangkat dinyalakan.

---

## Perbandingan HTTP dan MQTT

| Aspek | HTTP | MQTT |
|---|---|---|
| Pola komunikasi | Request-response | Publish-subscribe |
| Perantara | Server | Broker |
| Pengiriman | POST/GET | Publish/Subscribe |
| Overhead | Relatif lebih besar | Relatif kecil |
| Koneksi | Tidak harus selalu terbuka | Mendukung koneksi persistent |
| Pengamatan | Serial Monitor | MQTT Explorer |

## Kesimpulan

Praktikum berhasil menunjukkan bahwa ESP8266 dapat melakukan komunikasi data menggunakan **HTTP dan MQTT** dengan format **JSON**. HTTP berhasil mengirim data ke endpoint pengujian dengan response code **200**, sedangkan MQTT berhasil mempublikasikan data ke broker dan menerimanya melalui MQTT Explorer. Modifikasi `millis()` menambahkan informasi waktu berjalan perangkat ke dalam data JSON.

## Dokumentasi Prosesi Praktikum

<img width="1280" height="722" alt="WhatsApp Image 2026-09-16 at 23 58 31 (1)" src="https://github.com/user-attachments/assets/a84eb853-8d83-4ef9-a9ec-5695bbe8a895" />
<img width="1280" height="722" alt="WhatsApp Image 2026-09-16 at 23 58 31" src="https://github.com/user-attachments/assets/62c053e4-8d6a-4b27-8777-74962f6427c5" />

## Output Percobaan 

<img width="703" height="490" alt="Screenshot from 2026-09-16 11-31-59" src="https://github.com/user-attachments/assets/469ec549-b41d-4a74-8af5-ca0b655958b3" />
<img width="1029" height="607" alt="Screenshot from 2026-09-16 11-48-17" src="https://github.com/user-attachments/assets/61b269a9-4db7-4c9d-8080-770dd73604a6" />




