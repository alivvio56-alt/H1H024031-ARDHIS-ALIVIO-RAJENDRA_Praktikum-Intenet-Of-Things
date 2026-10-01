# Praktikum IoT — Percobaan 4A dan 4B

## 1. Deskripsi Praktikum

Praktikum ini membahas **komunikasi dan pertukaran data pada sistem IoT menggunakan protokol MQTT**. MQTT menggunakan mekanisme komunikasi **publish-subscribe**, yaitu perangkat dapat mengirim pesan melalui proses publish dan menerima pesan melalui proses subscribe dengan bantuan MQTT broker sebagai perantara.

Pada Percobaan 4A, fokus utama adalah penerimaan perintah melalui MQTT menggunakan mekanisme subscribe. ESP8266 menerima pesan dalam format JSON, melakukan deserialisasi, kemudian menggunakan data tersebut untuk mengendalikan LED sebagai aktuator.

Pada Percobaan 4B, komunikasi dikembangkan menjadi komunikasi dua arah. ESP8266 tidak hanya menerima perintah, tetapi juga melakukan publish data secara berkala. Program menggunakan `client.loop()` untuk memproses komunikasi MQTT dan `millis()` agar proses publish dapat dilakukan secara non-blocking.

Modifikasi dilakukan pada kedua percobaan untuk memperluas fungsi sistem:

* **Percobaan 4A:** menambahkan nilai `intensitas` pada JSON untuk mengatur kecerahan LED menggunakan PWM.
* **Percobaan 4B:** menambahkan topic baru untuk mengendalikan aktuator kedua berupa buzzer dan membuat callback dapat membedakan topic yang menerima pesan.

---

# 2. Tujuan Praktikum

Tujuan praktikum adalah:

1. Memahami komunikasi MQTT menggunakan mekanisme publish dan subscribe.
2. Memahami fungsi MQTT broker sebagai perantara komunikasi.
3. Memahami penggunaan topic untuk mengirim dan menerima pesan.
4. Memahami penggunaan callback dalam menerima pesan MQTT.
5. Memahami proses serialisasi dan deserialisasi JSON.
6. Mengendalikan aktuator berdasarkan data JSON yang diterima.
7. Menerapkan komunikasi dua arah pada perangkat IoT.
8. Menerapkan mekanisme non-blocking menggunakan `millis()`.
9. Mengembangkan program agar dapat mengendalikan lebih dari satu aktuator.

---

# 3. Perangkat dan Software

## 3.1 Perangkat Keras

Perangkat yang digunakan dalam percobaan adalah:

* ESP8266
* LED
* Resistor LED
* Buzzer untuk modifikasi Percobaan 4B
* Kabel USB
* Komputer/laptop

## 3.2 Software

Software yang digunakan:

* Arduino IDE
* MQTT Explorer
* MQTT Broker
* Serial Monitor

Broker MQTT yang digunakan:

```text
broker.hivemq.com
```

Port MQTT:

```text
1883
```

---

# 4. Library dan Dependencies

Program menggunakan beberapa library berikut.

## 4.1 ESP8266WiFi

```cpp
#include <ESP8266WiFi.h>
```

Library ini digunakan untuk menghubungkan ESP8266 ke jaringan WiFi.

Fungsi yang digunakan antara lain:

```cpp
WiFi.begin(ssid, password);
```

untuk memulai koneksi WiFi.

Kemudian:

```cpp
WiFi.status()
```

digunakan untuk memeriksa status koneksi WiFi.

---

## 4.2 PubSubClient

```cpp
#include <PubSubClient.h>
```

Library ini digunakan untuk komunikasi MQTT.

Fungsi utama yang digunakan:

```cpp
client.setServer()
```

untuk menentukan alamat broker dan port MQTT.

```cpp
client.connect()
```

untuk menghubungkan ESP8266 dengan broker.

```cpp
client.subscribe()
```

untuk mendaftarkan ESP8266 sebagai subscriber.

```cpp
client.publish()
```

untuk mengirim pesan MQTT.

```cpp
client.loop()
```

untuk memproses komunikasi MQTT secara terus-menerus.

```cpp
client.setCallback()
```

untuk mendaftarkan fungsi yang akan dipanggil ketika pesan MQTT diterima.

---

## 4.3 ArduinoJson

```cpp
#include <ArduinoJson.h>
```

Library ini digunakan untuk mengolah data dalam format JSON.

Pada program digunakan:

```cpp
deserializeJson()
```

untuk mengubah JSON yang diterima menjadi objek yang dapat diproses oleh program.

Sedangkan pada Percobaan 4B digunakan:

```cpp
serializeJson()
```

untuk mengubah data menjadi format JSON sebelum dipublikasikan melalui MQTT.

---

# 5. Percobaan 4A

## 5.1 Detail Percobaan

Percobaan 4A berfokus pada mekanisme **subscribe dan penerimaan perintah MQTT**.

ESP8266 terhubung ke WiFi dan MQTT broker, kemudian melakukan subscribe terhadap topic perintah.

Alur komunikasi:

```text
MQTT Explorer
      |
      | PUBLISH
      v
MQTT Broker
      |
      | SUBSCRIBE
      v
ESP8266
      |
      v
callback()
      |
      v
deserializeJson()
      |
      v
Perintah
      |
   +--+--+
   |     |
  ON    OFF
   |     |
   v     v
 LED ON LED OFF
```

---

# 6. Modifikasi Percobaan 4A

Pada program awal, pesan JSON hanya digunakan untuk menentukan kondisi LED:

```json
{
  "perintah": "ON"
}
```

Program kemudian dimodifikasi agar JSON juga memiliki parameter `intensitas`:

```json
{
  "perintah": "ON",
  "intensitas": 200
}
```

Nilai `intensitas` digunakan sebagai nilai PWM untuk mengatur kecerahan LED.

Rentang PWM:

```text
0   → LED mati
255 → intensitas maksimum
```

Contoh:

```json
{
  "perintah": "ON",
  "intensitas": 50
}
```

LED menyala dengan intensitas rendah.

```json
{
  "perintah": "ON",
  "intensitas": 128
}
```

LED menyala dengan intensitas menengah.

```json
{
  "perintah": "ON",
  "intensitas": 255
}
```

LED menyala dengan intensitas maksimum.

---

# 7. Penjelasan Code Percobaan 4A

## 7.1 Konfigurasi WiFi

```cpp
const char* ssid = "Authentic Nasgor Tuna Asap";
const char* password = "12345678";
```

Bagian tersebut menyimpan nama jaringan WiFi dan password yang digunakan ESP8266 untuk melakukan koneksi.

---

## 7.2 Konfigurasi MQTT

```cpp
const char* mqttServer = "broker.hivemq.com";
const int mqttPort = 1883;
```

Kode tersebut menentukan broker MQTT dan port yang digunakan.

Topic perintah:

```cpp
const char* topicPerintah =
    "unsoed/tk245004/viodupan/perintah";
```

Topic tersebut digunakan sebagai alamat pesan yang akan diterima oleh ESP8266.

---

## 7.3 Konfigurasi LED

```cpp
const int ledPin = 5;
```

GPIO 5 digunakan sebagai pin output LED pada ESP8266.

---

## 7.4 Membuat MQTT Client

```cpp
WiFiClient espClient;
PubSubClient client(espClient);
```

`WiFiClient` menyediakan koneksi jaringan yang digunakan oleh MQTT.

`PubSubClient` menggunakan koneksi tersebut untuk melakukan komunikasi dengan broker MQTT.

---

# 8. Fungsi `callback()`

```cpp
void callback(char* topic, byte* payload, unsigned int length)
```

Fungsi `callback()` dipanggil secara otomatis ketika ESP8266 menerima pesan pada topic yang telah di-subscribe.

Pertama, payload MQTT diubah menjadi `String`:

```cpp
String pesan;

for (unsigned int i = 0; i < length; i++) {
    pesan += (char)payload[i];
}
```

Payload kemudian dapat ditampilkan pada Serial Monitor.

---

## 8.1 Deserialisasi JSON

```cpp
JsonDocument doc;

DeserializationError error =
    deserializeJson(doc, pesan);
```

Data JSON yang diterima diubah menjadi objek `doc`.

Kemudian program mengecek apakah proses parsing berhasil:

```cpp
if (error) {
    Serial.print("Gagal parsing JSON: ");
    Serial.println(error.c_str());
    return;
}
```

Jika JSON tidak valid, program menampilkan error dan menghentikan pemrosesan pesan tersebut.

---

## 8.2 Mengambil Perintah

```cpp
const char* perintah = doc["perintah"];
```

Baris tersebut mengambil nilai dari key `perintah`.

Contoh:

```json
{
  "perintah": "ON"
}
```

maka:

```text
perintah = ON
```

---

## 8.3 Mengambil Intensitas

Pada modifikasi ditambahkan:

```cpp
int intensitas = doc["intensitas"];
```

Baris tersebut mengambil nilai `intensitas` dari JSON.

Contoh:

```json
{
  "perintah": "ON",
  "intensitas": 200
}
```

maka nilai:

```text
intensitas = 200
```

---

# 9. Percabangan Percobaan 4A

Percabangan digunakan untuk menentukan tindakan berdasarkan nilai `perintah`.

```cpp
if (String(perintah) == "ON") {
    analogWrite(ledPin, intensitas);
}
else if (String(perintah) == "OFF") {
    analogWrite(ledPin, 0);
}
```

Jika:

```text
perintah = ON
```

maka nilai `intensitas` diberikan ke LED menggunakan PWM.

Jika:

```text
perintah = OFF
```

maka PWM diatur menjadi `0`, sehingga LED mati.

Dengan demikian, conditional tidak hanya menentukan ON/OFF, tetapi pada kondisi ON juga menentukan tingkat kecerahan LED.

---

# 10. Fungsi `hubungkanWiFi()`

```cpp
void hubungkanWiFi()
```

Fungsi ini digunakan untuk menghubungkan ESP8266 ke jaringan WiFi.

```cpp
WiFi.begin(ssid, password);
```

memulai koneksi WiFi.

Kemudian:

```cpp
while (WiFi.status() != WL_CONNECTED)
```

digunakan untuk menunggu sampai ESP8266 berhasil terhubung.

---

# 11. Fungsi `hubungkanMQTT()`

```cpp
void hubungkanMQTT()
```

Fungsi ini digunakan untuk menghubungkan ESP8266 dengan MQTT broker.

Setelah berhasil terhubung:

```cpp
client.subscribe(topicPerintah);
```

ESP8266 melakukan subscribe terhadap topic perintah.

Subscribe dilakukan di dalam fungsi koneksi MQTT agar ketika terjadi reconnect, ESP8266 dapat melakukan subscribe kembali.

---

# 12. Fungsi `setup()`

Fungsi `setup()` hanya dijalankan satu kali ketika ESP8266 dinyalakan.

Fungsi ini digunakan untuk:

1. Memulai Serial Monitor.
2. Mengatur pin LED sebagai output.
3. Mengatur kondisi awal LED.
4. Menghubungkan WiFi.
5. Mengatur MQTT broker.
6. Mendaftarkan callback.

Contoh:

```cpp
client.setServer(mqttServer, mqttPort);
client.setCallback(callback);
```

---

# 13. Fungsi `loop()` pada 4A

```cpp
void loop()
```

digunakan untuk menjalankan program secara berulang.

Bagian penting:

```cpp
if (!client.connected()) {
    hubungkanMQTT();
}

client.loop();
```

Jika MQTT terputus, ESP8266 akan mencoba melakukan koneksi kembali.

`client.loop()` kemudian memproses komunikasi MQTT dan pesan yang masuk.

---

# 14. Percobaan 4B

## 14.1 Detail Percobaan

Percobaan 4B mengembangkan komunikasi menjadi **dua arah**.

ESP8266 dapat:

1. Menerima perintah melalui subscribe.
2. Mengendalikan aktuator.
3. Mengirim data melalui publish.
4. Melakukan proses komunikasi secara berkala.

Alurnya:

```text
                 MQTT BROKER
                /           \
               /             \
              v               v
        MQTT Explorer       ESP8266
              |               |
              |               |
           Publish          Publish
              |               |
              v               v
           ESP8266        MQTT Explorer
```

Dengan demikian ESP8266 memiliki fungsi sebagai **subscriber sekaligus publisher**.

---

# 15. Modifikasi Percobaan 4B

Modifikasi dilakukan dengan menambahkan **satu topic perintah baru** untuk mengendalikan aktuator kedua berupa buzzer.

Topic yang digunakan:

```text
LED:
unsoed/tk245004/viodupan/led

Buzzer:
unsoed/tk245004/viodupan/buzzer

Data:
unsoed/tk245004/viodupan/data
```

ESP8266 melakukan subscribe terhadap topic LED dan topic buzzer.

---

# 16. Penambahan Topic Buzzer

```cpp
const char* topicBuzzer =
    "unsoed/tk245004/viodupan/buzzer";
```

Variabel tersebut menyimpan alamat topic khusus untuk perintah buzzer.

Dengan adanya topic berbeda, pesan untuk LED dan buzzer dapat dipisahkan.

---

# 17. Penambahan Pin Buzzer

```cpp
const int buzzerPin = 4;
```

GPIO 4 digunakan sebagai pin output untuk buzzer.

Pada `setup()`:

```cpp
pinMode(buzzerPin, OUTPUT);
digitalWrite(buzzerPin, LOW);
```

GPIO buzzer diatur sebagai output dan kondisi awal buzzer dibuat mati.

---

# 18. Subscribe Dua Topic

Pada fungsi `hubungkanMQTT()` ditambahkan:

```cpp
client.subscribe(topicLED);
client.subscribe(topicBuzzer);
```

ESP8266 sekarang terdaftar sebagai subscriber pada dua topic.

Dengan demikian, callback dapat menerima pesan dari kedua topic tersebut.

---

# 19. Callback pada Percobaan 4B

Callback tetap digunakan untuk menerima pesan:

```cpp
void callback(char* topic, byte* payload, unsigned int length)
```

Perbedaannya adalah callback sekarang memeriksa nilai `topic`.

Untuk membedakan topic digunakan:

```cpp
strcmp(topic, topicLED)
```

dan:

```cpp
strcmp(topic, topicBuzzer)
```

`strcmp()` digunakan untuk membandingkan dua string.

---

# 20. Percabangan Topic

Conditional utama pada modifikasi 4B:

```cpp
if (strcmp(topic, topicLED) == 0) {
    // kontrol LED
}
else if (strcmp(topic, topicBuzzer) == 0) {
    // kontrol buzzer
}
```

Jika topic yang diterima sama dengan `topicLED`, program menjalankan bagian kontrol LED.

Jika topic yang diterima sama dengan `topicBuzzer`, program menjalankan bagian kontrol buzzer.

Dengan demikian, payload yang sama dapat menghasilkan tindakan berbeda karena topic digunakan sebagai pembeda.

---

# 21. Conditional LED

Jika pesan berasal dari topic LED:

```cpp
if (String(perintah) == "ON") {
    digitalWrite(ledPin, HIGH);
}
else if (String(perintah) == "OFF") {
    digitalWrite(ledPin, LOW);
}
```

`ON` menyebabkan LED menyala.

`OFF` menyebabkan LED mati.

---

# 22. Conditional Buzzer

Jika pesan berasal dari topic buzzer:

```cpp
if (String(perintah) == "ON") {
    digitalWrite(buzzerPin, HIGH);
}
else if (String(perintah) == "OFF") {
    digitalWrite(buzzerPin, LOW);
}
```

`ON` menyebabkan buzzer aktif.

`OFF` menyebabkan buzzer mati.

---

# 23. Fungsi `publishData()`

Pada Percobaan 4B, ESP8266 juga mengirim data melalui MQTT.

Data dibuat dalam format JSON:

```cpp
JsonDocument doc;

doc["device"] = "ESP8266";
doc["status"] = "ONLINE";
```

Kemudian diubah menjadi String JSON:

```cpp
String jsonData;

serializeJson(doc, jsonData);
```

Setelah itu dikirim ke broker:

```cpp
client.publish(topicData, jsonData.c_str());
```

Dengan demikian alurnya:

```text
Data
  ↓
JsonDocument
  ↓
serializeJson()
  ↓
JSON String
  ↓
client.publish()
  ↓
MQTT Broker
```

---

# 24. Penggunaan `millis()`

Pada Percobaan 4B digunakan mekanisme non-blocking:

```cpp
unsigned long waktuTerakhirPublish = 0;
const unsigned long intervalPublish = 5000;
```

Kemudian:

```cpp
unsigned long waktuSekarang = millis();

if (waktuSekarang - waktuTerakhirPublish >=
    intervalPublish) {

    waktuTerakhirPublish = waktuSekarang;

    publishData();
}
```

Program tidak berhenti selama 5 detik seperti ketika menggunakan `delay(5000)`.

Selama menunggu interval publish, program tetap dapat menjalankan:

```cpp
client.loop();
```

sehingga pesan masuk tetap dapat diproses.

---

# 25. Conditional pada `loop()` Percobaan 4B

Pertama program memeriksa koneksi MQTT:

```cpp
if (!client.connected()) {
    hubungkanMQTT();
}
```

Jika MQTT tidak terhubung, fungsi `hubungkanMQTT()` dipanggil.

Setelah itu:

```cpp
client.loop();
```

tetap dijalankan untuk memproses komunikasi MQTT.

Kemudian program memeriksa apakah interval publish sudah tercapai:

```cpp
if (waktuSekarang - waktuTerakhirPublish >=
    intervalPublish)
```

Jika sudah tercapai, fungsi `publishData()` dipanggil.

---

# 26. Contoh Pengujian Percobaan 4B

## 26.1 Menghidupkan LED

Topic:

```text
unsoed/tk245004/viodupan/led
```

Payload:

```json
{
  "perintah": "ON"
}
```

Hasil:

```text
LED: ON
```

---

## 26.2 Mematikan LED

Topic:

```text
unsoed/tk245004/viodupan/led
```

Payload:

```json
{
  "perintah": "OFF"
}
```

Hasil:

```text
LED: OFF
```

---

## 26.3 Menghidupkan Buzzer

Topic:

```text
unsoed/tk245004/viodupan/buzzer
```

Payload:

```json
{
  "perintah": "ON"
}
```

Hasil:

```text
BUZZER: ON
```

---

## 26.4 Mematikan Buzzer

Topic:

```text
unsoed/tk245004/viodupan/buzzer
```

Payload:

```json
{
  "perintah": "OFF"
}
```

Hasil:

```text
BUZZER: OFF
```

---

# 27. Perbandingan Percobaan 4A dan 4B

| Aspek           | Percobaan 4A              | Percobaan 4B                    |
| --------------- | ------------------------- | ------------------------------- |
| Fokus           | Subscribe dan kontrol LED | Komunikasi dua arah             |
| Publish         | Tidak menjadi fokus utama | Ya                              |
| Subscribe       | Ya                        | Ya                              |
| Aktuator        | LED                       | LED + buzzer setelah modifikasi |
| JSON            | Diterima                  | Diterima dan dikirim            |
| Deserialisasi   | Ya                        | Ya                              |
| Serialisasi     | Tidak menjadi fokus       | Ya                              |
| `callback()`    | Ya                        | Ya                              |
| `client.loop()` | Ya                        | Ya                              |
| `millis()`      | Tidak menjadi fokus       | Ya                              |
| Topic perintah  | Satu                      | Dua setelah modifikasi          |

---

# 28. Alur Keseluruhan Sistem

```text
                    MQTT BROKER
                   /           \
                  /             \
                 v               v
          MQTT Explorer       ESP8266
                |                |
                | Publish        | Publish
                |                |
                v                v
             ESP8266         MQTT Explorer
                |
                v
             callback()
                |
                v
        deserializeJson()
                |
                v
          Cek topic pesan
           /           \
          /             \
      Topic LED       Topic Buzzer
         |                |
         v                v
       LED ON/OFF      Buzzer ON/OFF
```

Pada saat yang sama, ESP8266 dapat melakukan publish data secara berkala menggunakan `millis()`.

---

# 29. Kesimpulan

Percobaan 4A menunjukkan penerapan komunikasi MQTT dengan mekanisme subscribe untuk menerima perintah dari broker. Data yang diterima dalam format JSON diproses menggunakan deserialisasi, kemudian digunakan untuk mengendalikan LED.

Modifikasi 4A menambahkan parameter `intensitas` sehingga LED dapat dikendalikan menggunakan PWM. Dengan demikian, LED tidak hanya memiliki kondisi ON dan OFF, tetapi juga dapat memiliki tingkat kecerahan yang berbeda.

Percobaan 4B mengembangkan komunikasi menjadi dua arah dengan menggabungkan mekanisme publish dan subscribe. ESP8266 dapat menerima perintah sekaligus mengirimkan data secara berkala. Penggunaan `millis()` memungkinkan proses publish dilakukan secara non-blocking sehingga `client.loop()` tetap dapat berjalan.

Modifikasi 4B menambahkan topic baru untuk aktuator kedua berupa buzzer. Fungsi `callback()` menggunakan informasi topic dan `strcmp()` untuk membedakan pesan yang ditujukan kepada LED atau buzzer. Dengan demikian, satu perangkat ESP8266 dapat menerima perintah dari beberapa topic dan mengendalikan aktuator yang berbeda sesuai dengan topic yang digunakan.
