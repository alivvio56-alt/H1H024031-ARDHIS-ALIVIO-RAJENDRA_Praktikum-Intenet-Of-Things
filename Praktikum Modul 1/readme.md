# Praktikum IoT I — Sensor dan Aktuator

**Mata Kuliah:** Praktikum Internet of Things (TK245002)  
**Modul:** 1 — Sensor dan Aktuator  
**Topik:** Akuisisi Data Sensor dan Kendali Aktuator  
**Mikrokontroler:** ESP8266  
**Sensor:** DHT11  
**Aktuator:** Modul Relay  
**Tahun/Semester:** 2026 / 5

---

## Identitas

| Data | Keterangan |
|---|---|
| Nama | Ardhis Alivio Rajendra |
| NIM | H1H024031 |
| Asisten | Athallah Tsany Satryaji |

---

## 1. Deskripsi Praktikum

Praktikum Modul 1 membahas mengenai akuisisi data sensor dan kendali aktuator pada sistem Internet of Things. Pada implementasinya digunakan ESP8266 sebagai mikrokontroler, DHT11 sebagai sensor untuk membaca suhu dan kelembaban, serta modul relay sebagai aktuator.

Alur sistem:

```
    Lingkungan
        |
        v
      DHT11
        |
  Suhu & Kelembaban
        |
        v
     ESP8266
        |
  Pengolahan Data
        |
        v
      Relay
        |
        v
    Aksi ON/OFF
```

---

## 2. Tujuan Praktikum

Tujuan praktikum:

- Memahami proses akuisisi data sensor menggunakan ESP8266.
- Memahami kendali aktuator menggunakan ESP8266.
- Membaca suhu dan kelembaban menggunakan sensor DHT11.
- Mengendalikan relay berdasarkan data sensor.
- Menganalisis hubungan antara data sensor dengan respons aktuator.

---

## 3. Dasar Teori

### 3.1 Internet of Things (IoT)

Internet of Things (IoT) adalah konsep sistem yang menghubungkan perangkat untuk memperoleh data dari lingkungan, memproses data, dan menghasilkan suatu tindakan berdasarkan data tersebut.

Pada praktikum ini:

```
DHT11 -> ESP8266 -> Relay
```

### 3.2 Akuisisi Data Sensor

Akuisisi data sensor adalah proses mengambil data dari lingkungan menggunakan sensor untuk kemudian diproses oleh mikrokontroler.

Pada praktikum ini, DHT11 digunakan untuk mengambil:
- Data suhu
- Data kelembaban

### 3.3 Sensor Digital

Sensor digital menghasilkan data dalam bentuk sinyal digital yang dapat dibaca oleh mikrokontroler.

Contoh sensor digital:
- DHT11
- DHT22
- PIR
- HC-SR04

Pada praktikum digunakan DHT11.

### 3.4 Aktuator

Aktuator adalah perangkat yang mengubah perintah elektrik dari mikrokontroler menjadi suatu aksi fisik.

Contoh aktuator:
- Relay
- Motor servo
- Buzzer

### 3.5 Relay

Relay merupakan saklar elektromekanis yang dapat dikendalikan oleh mikrokontroler. Relay dapat digunakan untuk mengendalikan perangkat seperti:
- Lampu
- Kipas
- Pompa

---

## 4. Komponen

Komponen yang digunakan:

- ESP8266
- Sensor DHT11
- Modul relay
- Breadboard
- Kabel jumper
- Beban/indikator

---

## 5. Konfigurasi Pin

Konfigurasi pin yang digunakan pada program:

| Komponen | Pin ESP8266 |
|---|---|
| DHT11 Data | GPIO 4 |
| Relay | SCL [D1] |


---

## 6. Library / Dependencies

Library yang digunakan:

```cpp
#include <DHT.h>
```

Library DHT digunakan untuk membaca data suhu dan kelembaban dari sensor DHT11.

Fungsi yang digunakan:

| Fungsi | Keterangan |
|---|---|
| `dht.begin()` | Menginisialisasi sensor DHT |
| `dht.readTemperature()` | Membaca suhu |
| `dht.readHumidity()` | Membaca kelembaban |
| `isnan()` | Mengecek validitas data sensor |

---

## 7. Percobaan 1 — Pembacaan Sensor DHT11

### 7.1 Tujuan

Mengamati pembacaan suhu dan kelembaban dari sensor DHT11 pada beberapa kondisi lingkungan.

### 7.2 Rangkaian
<img width="468" height="395" alt="Screenshot from 2026-09-02 16-07-46" src="https://github.com/user-attachments/assets/e74ed1db-ce5c-4713-8f38-e7c6f02abb54" />

[Foto rangkaian Percobaan 1]

### 7.3 Kondisi Pengujian

Sensor diuji pada beberapa kondisi:
- Kondisi normal
- Sensor didekati tangan
- Sensor didekati AC
- Sensor didekati api

### 7.4 Hasil Pengamatan

| No. | Waktu | Kondisi Sensor | Suhu (°C) | Kelembaban (%) | Status Pembacaan |
|---|---|---|---|---|---|
| 1 | 00.00 | Normal | 28,00 | 47,00 | Valid |
| 2 | 00.02 | Didekati tangan | 29,00 | 82,00 | Valid |
| 3 | 00.04 | Didekati AC | 23,80 | 54,00 | Valid |
| 4 | 00.10 | Didekati api | 30,20 | 50,00 | Valid |

### 7.5 Analisis

Berdasarkan hasil pengamatan, sensor DHT11 berhasil membaca data suhu dan kelembaban pada setiap kondisi dengan status pembacaan valid.

Pada kondisi normal diperoleh suhu sebesar 28°C dan kelembaban 47%. Ketika sensor didekati tangan, suhu meningkat menjadi 29°C dan kelembaban meningkat menjadi 82%.

Ketika sensor didekati AC, suhu turun menjadi 23,80°C dengan kelembaban 54%. Perubahan tersebut menunjukkan bahwa kondisi lingkungan di sekitar sensor memengaruhi nilai suhu dan kelembaban yang terbaca oleh sensor.

---

## 8. Percobaan 2 — Kendali Relay Berdasarkan Sensor

### 8.1 Tujuan

Mengendalikan relay berdasarkan nilai suhu yang dibaca oleh sensor DHT11.

### 8.2 Rangkaian

<img width="468" height="430" alt="Screenshot from 2026-09-02 16-22-26" src="https://github.com/user-attachments/assets/16745a71-919f-458c-952e-b9e05a5f889d" />

[Foto rangkaian Percobaan 2]

### 8.3 Logika Program

Program menggunakan threshold suhu sebesar 30°C.

```
Suhu < 30°C
    |
    v
Relay OFF

Suhu > 30°C
    |
    v
Relay ON
```

### 8.4 Hasil Pengamatan

| No. | Waktu | Suhu (°C) | Threshold (°C) | Kondisi Suhu | Status Aktuator | Kondisi LED/Relay |
|---|---|---|---|---|---|---|
| 1 | 00.00 | 28,00 | 30 | < threshold | OFF | Menyala |
| 2 | 00.02 | 30,20 | 30 | > threshold | ON | Menyala |
| 3 | 00.04 | 30,20 | 30 | > threshold | ON | Menyala |

### 8.5 Analisis

Berdasarkan hasil pengamatan, relay memberikan respons berdasarkan data suhu yang dibaca oleh sensor.

Pada suhu 28°C, nilai suhu berada di bawah threshold 30°C sehingga status aktuator adalah OFF. Ketika suhu meningkat menjadi 30,20°C, nilai tersebut melewati threshold sehingga status aktuator berubah menjadi ON.

Dengan demikian, data suhu dari sensor dapat digunakan sebagai dasar untuk mengendalikan aktuator secara otomatis.

---

## 9. Penjelasan Program

### 9.1 Library DHT

```cpp
#include <DHT.h>
```

Digunakan untuk memasukkan library DHT sehingga mikrokontroler dapat berkomunikasi dengan sensor DHT11.

### 9.2 Konfigurasi Sensor

```cpp
#define DHTPIN 4
#define DHTTYPE DHT11
```

- `DHTPIN 4` menentukan pin data sensor.
- `DHTTYPE DHT11` menentukan jenis sensor yang digunakan.

### 9.3 Konfigurasi Relay

```cpp
#define RELAYPIN 26
```

Menentukan pin yang digunakan untuk mengendalikan relay.

### 9.4 Membuat Objek DHT

```cpp
DHT dht(DHTPIN, DHTTYPE);
```

Membuat objek dht berdasarkan pin dan jenis sensor yang telah ditentukan.

### 9.5 Threshold

```cpp
const float suhuThreshold = 30.0;
```

Menentukan nilai batas suhu sebesar 30°C sebagai acuan untuk mengendalikan relay.

### 9.6 Inisialisasi

```cpp
void setup() {
    pinMode(RELAYPIN, OUTPUT);
    dht.begin();
    Serial.begin(115200);
}
```

Fungsi:
- `pinMode()` — mengatur pin relay sebagai output.
- `dht.begin()` — memulai komunikasi dengan sensor.
- `Serial.begin()` — memulai komunikasi Serial Monitor.

### 9.7 Membaca Suhu

```cpp
float suhu = dht.readTemperature();
```

Digunakan untuk membaca suhu dari sensor DHT11.

### 9.8 Membaca Kelembaban

```cpp
float kelembaban = dht.readHumidity();
```

Digunakan untuk membaca kelembaban dari sensor DHT11.

### 9.9 Pemeriksaan Data

```cpp
if (isnan(suhu) || isnan(kelembaban)) {
    Serial.println("Gagal membaca sensor");
}
```

`isnan()` digunakan untuk memeriksa apakah hasil pembacaan sensor valid. Jika hasil pembacaan berupa NaN, maka data sensor dianggap tidak valid.

### 9.10 Percabangan Relay

```cpp
if (suhu > suhuThreshold) {
    digitalWrite(RELAYPIN, HIGH);
} else {
    digitalWrite(RELAYPIN, LOW);
}
```

Percabangan digunakan untuk menentukan kondisi relay:

```
Suhu > 30°C
    |
    v
Relay ON

Suhu <= 30°C
    |
    v
Relay OFF
```

---

## 10. Kendali Histerisis

Program kemudian dimodifikasi menggunakan metode histerisis dengan dua threshold.

Batas yang digunakan:
- Relay ON — Suhu > 30°C
- Relay OFF — Suhu < 28°C

Jika suhu berada pada rentang 28°C sampai 30°C, kondisi relay tetap seperti kondisi sebelumnya.

Alur:

```
      Suhu > 30°C
          |
          v
      RELAY ON
          |
          v
     Suhu 28-30°C
          |
   Kondisi tetap
          |
          v
      Suhu < 28°C
          |
          v
     RELAY OFF
```

Histerisis digunakan agar aktuator tidak terlalu sering berpindah antara ON dan OFF ketika suhu berada di sekitar threshold.

---

## 11. Source Code Histerisis

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
    
    // Memulai komunikasi dengan sensor DHT11
    dht.begin();
    
    // Kondisi awal relay OFF
    digitalWrite(RELAYPIN, LOW);
    
    // Memulai komunikasi Serial Monitor
    Serial.begin(115200);
}

void loop() {
    // Membaca suhu dari sensor DHT11
    float suhu = dht.readTemperature();
    
    // Mengecek apakah pembacaan sensor valid
    if (isnan(suhu)) {
        Serial.println("Gagal membaca sensor DHT11");
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

---

## 12. Penjelasan Source Code Histerisis

| Bagian Kode | Penjelasan |
|---|---|
| `#include <DHT.h>` | Memasukkan library DHT. |
| `#define DHTPIN 4` | Menentukan pin data sensor pada GPIO 4. |
| `#define DHTTYPE DHT11` | Menentukan sensor yang digunakan adalah DHT11. |
| `#define RELAYPIN 26` | Menentukan pin kendali relay. |
| `DHT dht(...)` | Membuat objek sensor DHT. |
| `suhuON = 30.0` | Batas untuk menyalakan relay. |
| `suhuOFF = 28.0` | Batas untuk mematikan relay. |
| `relayState` | Menyimpan kondisi relay sebelumnya. |
| `pinMode()` | Mengatur pin relay sebagai output. |
| `dht.begin()` | Menginisialisasi sensor DHT11. |
| `digitalWrite()` | Mengatur kondisi output relay. |
| `readTemperature()` | Membaca suhu. |
| `isnan()` | Mengecek validitas pembacaan sensor. |
| `suhu > suhuON` | Menyalakan relay ketika suhu > 30°C. |
| `suhu < suhuOFF` | Mematikan relay ketika suhu < 28°C. |
| `relayState` | Mempertahankan kondisi relay pada rentang 28-30°C. |
| `delay(2000)` | Memberikan jeda 2 detik antar pembacaan. |

---

## 13. Jawaban Pertanyaan Praktikum

### 13.1 Mengapa diperlukan nilai threshold?

Threshold diperlukan sebagai batas acuan untuk menentukan kapan aktuator menyala atau mati berdasarkan nilai sensor. Dengan threshold, ESP8266 dapat mengambil keputusan secara otomatis berdasarkan kondisi sensor.

Contoh:
- Suhu > 30°C → Relay ON
- Suhu ≤ 30°C → Relay OFF

### 13.2 Apa yang terjadi apabila suhuThreshold diturunkan menjadi 20°C?

Jika threshold diturunkan menjadi 20°C, maka pada kondisi suhu ruangan yang berada di atas 20°C, relay akan lebih sering atau terus menyala karena suhu telah melewati threshold.

Contoh:
```
Suhu      = 28°C
Threshold = 20°C
28 > 20
  |
  v
Relay ON
```

### 13.3 Apa perbedaan kendali kondisi tunggal dengan histerisis?

**Kondisi tunggal:**
- Menggunakan satu threshold untuk menentukan kondisi ON atau OFF.
- Suhu > 30°C → ON
- Suhu ≤ 30°C → OFF

**Histerisis:**
- Menggunakan dua threshold, yaitu batas atas untuk ON dan batas bawah untuk OFF.
- Suhu > 30°C → ON
- Suhu < 28°C → OFF
- 28–30°C → Kondisi sebelumnya

Histerisis membuat sistem lebih stabil karena mencegah aktuator terlalu sering berubah kondisi ketika suhu berada di sekitar threshold.

---

## 14. Dokumentasi Praktikum

### 14.1 Foto Rangkaian Percobaan 1

<img width="1280" height="722" alt="WhatsApp Image 2026-09-02 at 16 09 18" src="https://github.com/user-attachments/assets/674f53a1-e847-410b-b8ba-0773d6409925" />


### 14.2 Foto Rangkaian Percobaan 2

<img width="960" height="1280" alt="WhatsApp Image 2026-09-02 at 16 57 01" src="https://github.com/user-attachments/assets/32b09ff9-3731-47d0-835e-bbc5279ca2fd" />


### 14.3 Foto Proses Praktikum

<img width="1076" height="1600" alt="WhatsApp Image 2026-09-05 at 20 15 26" src="https://github.com/user-attachments/assets/9fcbc676-fb60-4f62-b42d-f7bfde7f39c6" />

---

## 15. Kendala Praktikum

### Kendala 1 — Perbedaan Konfigurasi Sensor

Pada awal perakitan, terdapat perbedaan antara informasi pada datasheet sensor dengan konfigurasi sensor yang tersedia di laboratorium. Hal tersebut menyebabkan kesalahan pemasangan pin sehingga sensor tidak dapat membaca data dengan baik.

**Solusi:**
Melakukan pengecekan kembali konfigurasi pin sensor yang digunakan di laboratorium sebelum melakukan perakitan.

### Kendala 2 — Tegangan Relay

Pada percobaan kedua, tegangan keluaran ESP hanya sebesar 3,3 V, sedangkan relay diperkirakan membutuhkan tegangan 5 V untuk mengaktifkan mekanisme saklarnya. Akibatnya, lampu indikator relay menyala, tetapi tidak terdengar bunyi klik dan mekanisme saklar tidak berpindah.

**Solusi:**
Melakukan pengecekan kebutuhan tegangan modul relay dan menyesuaikan sumber tegangan agar relay dapat bekerja sesuai spesifikasinya.

---

## 16. Hasil Pengamatan Umum

Berdasarkan hasil pengamatan, sensor DHT11 berhasil membaca suhu dan kelembaban pada berbagai kondisi dengan status pembacaan valid. Pada kondisi normal diperoleh suhu 28°C dan kelembaban 47%. Ketika sensor didekati tangan, suhu meningkat menjadi 29°C dan kelembaban menjadi 82%.

Saat sensor didekati AC, suhu turun menjadi 23,80°C dengan kelembaban 54%. Ketika sensor didekati api, suhu meningkat menjadi 30,20°C dengan kelembaban 50%. Hasil tersebut menunjukkan bahwa perubahan kondisi lingkungan di sekitar sensor memengaruhi nilai suhu dan kelembaban yang terbaca.

---

## 17. Kesimpulan

Praktikum Modul 1 menunjukkan bahwa ESP8266 dapat digunakan untuk melakukan akuisisi data dari sensor DHT11 dan mengendalikan aktuator relay berdasarkan data sensor.

Sensor DHT11 berhasil membaca perubahan suhu dan kelembaban pada beberapa kondisi lingkungan. Data suhu kemudian digunakan sebagai dasar pengambilan keputusan untuk mengendalikan relay menggunakan nilai threshold.

Penggunaan metode histerisis dengan batas ON sebesar 30°C dan batas OFF sebesar 28°C memberikan mekanisme kendali yang lebih stabil karena kondisi relay dapat dipertahankan ketika suhu berada di antara kedua batas tersebut.

---


## Author

Ardhis Alivio Rajendra  
H1H024031  
Teknik Komputer — Universitas Jenderal Soedirman
