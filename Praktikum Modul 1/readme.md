# Kendali Relay dengan Histerisis

## Deskripsi

Program ini digunakan untuk mengendalikan relay berdasarkan suhu yang dibaca oleh sensor DHT22 menggunakan ESP32.

Sistem menggunakan metode **histerisis** dengan dua batas:

- Relay **menyala** jika suhu **> 30°C**
- Relay **mati** jika suhu **< 28°C**
- Jika suhu berada di antara **28°C – 30°C**, kondisi relay tetap seperti sebelumnya (tidak berubah)

## Cara Kerja Histerisis

```
Suhu > 30°C
     ↓
  RELAY ON
     ↓
Suhu turun
     ↓
28–30°C
     ↓
RELAY tetap ON
     ↓
Suhu < 28°C
     ↓
 RELAY OFF
```

## Kebutuhan Hardware

- ESP32
- Sensor suhu & kelembapan DHT22
- Modul relay
- Kabel jumper

## Konfigurasi Pin

| Komponen  | Pin ESP32 |
|-----------|-----------|
| DHT11     | GPIO 4    |
| Relay     | GPIO 26   |

## Source Code

```cpp
#include <DHT.h>

// Pin DHT11
#define DHTPIN 4
// Jenis sensor
#define DHTTYPE DHT11
// Pin relay
#define RELAYPIN 26

// Membuat objek DHT
DHT dht(DHTPIN, DHTTYPE);

// Batas suhu untuk menyalakan relay
const float suhuON = 30.0;
// Batas suhu untuk mematikan relay
const float suhuOFF = 28.0;

// Menyimpan kondisi relay sebelumnya
bool relayState = false;

void setup() {
  // Mengatur pin relay sebagai output
  pinMode(RELAYPIN, OUTPUT);
  // Memulai komunikasi dengan sensor DHT22
  dht.begin();
  // Kondisi awal relay OFF
  digitalWrite(RELAYPIN, LOW);
  // Memulai komunikasi Serial Monitor
  Serial.begin(115200);
}

void loop() {
  // Membaca suhu dari sensor DHT22
  float suhu = dht.readTemperature();

  // Mengecek apakah pembacaan sensor valid
  if (isnan(suhu)) {
    Serial.println("Gagal membaca sensor DHT22");
    delay(2000);
    return;
  }

  // Jika suhu lebih dari 30°C, relay dinyalakan
  if (suhu > suhuON) {
    relayState = true;
  }
  // Jika suhu kurang dari 28°C, relay dimatikan
  else if (suhu < suhuOFF) {
    relayState = false;
  }

  // Mengirim kondisi relay ke pin relay
  digitalWrite(RELAYPIN, relayState ? HIGH : LOW);

  // Menampilkan data suhu pada Serial Monitor
  Serial.print("Suhu: ");
  Serial.print(suhu);
  Serial.print(" °C | Relay: ");

  // Menampilkan status relay
  if (relayState) {
    Serial.println("ON");
  } else {
    Serial.println("OFF");
  }

  // Memberikan jeda pembacaan
  delay(2000);
}
```

## Penjelasan Kode

| Kode | Fungsi |
|------|--------|
| `#include <DHT.h>` | Memasukkan library untuk sensor DHT22. |
| `#define DHTPIN 4` | Menentukan GPIO 4 sebagai pin data DHT22. |
| `#define DHTTYPE DHT22` | Menentukan jenis sensor yang digunakan adalah DHT22. |
| `#define RELAYPIN 26` | Menentukan GPIO 26 sebagai pin kendali relay. |
| `DHT dht(...)` | Membuat objek untuk komunikasi dengan DHT22. |
| `suhuON = 30.0` | Menentukan suhu untuk mengaktifkan relay. |
| `suhuOFF = 28.0` | Menentukan suhu untuk menonaktifkan relay. |
| `relayState` | Menyimpan kondisi relay sebelumnya. |
| `pinMode()` | Mengatur pin relay sebagai output. |
| `dht.begin()` | Memulai sensor DHT22. |
| `digitalWrite()` | Mengatur relay ON atau OFF. |
| `dht.readTemperature()` | Membaca suhu dari DHT22. |
| `isnan()` | Memeriksa apakah hasil pembacaan sensor valid. |
| `if (suhu > suhuON)` | Relay menjadi ON jika suhu > 30°C. |
| `else if (suhu < suhuOFF)` | Relay menjadi OFF jika suhu < 28°C. |
| `relayState` di antara 28–30°C | Relay mempertahankan kondisi sebelumnya. |
| `delay(2000)` | Memberikan jeda 2 detik sebelum pembacaan berikutnya. |

## Lisensi

Bebas digunakan dan dimodifikasi untuk keperluan pembelajaran maupun proyek pribadi.
