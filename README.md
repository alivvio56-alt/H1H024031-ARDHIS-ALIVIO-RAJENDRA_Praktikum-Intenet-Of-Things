# Praktikum Internet of Things

**Modul Praktikum Pemrograman Sistem Embedded dan IoT**

[![Status](https://img.shields.io/badge/Status-Active-brightgreen)](https://github.com)
[![License](https://img.shields.io/badge/License-MIT-blue)](LICENSE)
[![Contributor](https://img.shields.io/badge/Contributor-Ardhis%20Alivio%20Rajendra-informational)](https://github.com)

---

## Identitas

| Informasi | Detail |
|-----------|--------|
| **Nama** | Ardhis Alivio Rajendra |
| **NIM** | H1H024031 |
| **Mata Kuliah** | Praktikum Internet of Things (TK245002) |
| **Program Studi** | Teknik Komputer |
| **Universitas** | Universitas Jenderal Soedirman |
| **Tahun Ajaran** | 2026 / Semester 5 |

---

## Tentang Repository

Repository ini berisi dokumentasi lengkap dan source code untuk praktikum **Internet of Things (IoT)** pada mata kuliah Praktikum Sistem Internet of Things. Setiap modul mencakup teori, implementasi, dan hasil eksperimen dengan berbagai komponen hardware dan sensor.

---

## Apa itu Internet of Things (IoT)?

Internet of Things adalah paradigma komputasi yang menghubungkan perangkat fisik dengan internet untuk mengumpulkan, memproses, dan berbagi data secara otomatis. Sistem IoT terdiri dari tiga komponen utama:

```
Sensor (Input)  ->  Mikrokontroler (Processing)  ->  Aktuator (Output)
  [DHT11]              [ESP8266/Arduino]              [Relay/Motor/LED]
```

Dalam praktikum ini, kami mempelajari cara mengintegrasikan sensor dan aktuator dengan mikrokontroler untuk membangun sistem IoT yang responsif terhadap kondisi lingkungan.

---

## Ruang Lingkup Praktikum

Praktikum IoT mencakup pembelajaran tentang:

- **Akuisisi Data Sensor** — Membaca data dari berbagai jenis sensor digital dan analog
- **Pemrosesan Data** — Validasi, filtering, dan interpretasi data sensor
- **Kendali Aktuator** — Menggerakkan relay, motor, dan perangkat lainnya berdasarkan logika program
- **Komunikasi Mikrokontroler** — Serial communication, protokol wireless (WiFi/Bluetooth)
- **Sistem Kontrol** — Implementasi control logic sederhana hingga advanced (threshold, hysteresis, PID)
- **Integrasi Sistem** — Menggabungkan multiple sensor dan aktuator dalam satu aplikasi

---


## Software yang Dibutuhkan

- **Arduino IDE** — Programming environment untuk mikrokontroler
- **Library ** — Library untuk komunikasi dengan sensor 
- **USB Driver** — Driver untuk koneksi ESP8266 ke komputer

**Instalasi:**

```bash
# Clone repository
git clone https://github.com/yourusername/Praktikum-IoT.git

# Install dependencies (jika diperlukan)
# Arduino IDE: Sketch > Include Library > Manage Libraries
# Cari dan install: DHT sensor library by Adafruit
```

---


## Referensi & Resources

### Datasheet
- [ESP8266 Datasheet](https://www.espressif.com/sites/default/files/documentation/esp8266-technical_reference_en.pdf)
- [DHT11 Sensor Datasheet](https://www.mouser.com/datasheet/2/758/DHT11-9162-021517-v02-4755a3.pdf)

### Library Documentation
- [Adafruit DHT Library](https://github.com/adafruit/DHT-sensor-library)
- [ESP8266 Arduino Core](https://github.com/esp8266/Arduino)

### Tutorial & Guides
- Arduino Official Documentation: https://www.arduino.cc/
- ESP8266 Getting Started: https://github.com/esp8266/Arduino/blob/master/README.md

---

## Lisensi

Project ini dilisensikan di bawah MIT License — bebas digunakan, dimodifikasi, dan didistribusikan dengan mencantumkan atribusi penulis.

---

## Kontak & Informasi

**Praktikan:**
- Nama: Ardhis Alivio Rajendra
- NIM: H1H024031
- Program Studi: Teknik Komputer
- Universitas: Universitas Jenderal Soedirman

**Asisten Praktikum:**
- Athallah Tsany Satryaji

---

## Status Project

- [x] Modul 1 — Sensor dan Aktuator (Complete)
---

## Catatan Penting

Praktikum ini menggunakan perangkat hardware yang beroperasi pada tegangan listrik. Selalu perhatikan keselamatan kerja:

- Matikan power sebelum merangkai ulang komponen
- Gunakan breadboard untuk prototyping, hindari koneksi langsung pada power supply
- Periksa polaritas komponen sebelum power on
- Konsultasikan dengan asisten jika mengalami masalah

---

**Terakhir diperbarui:** September 2026


---

*Repository ini adalah bagian dari praktikum akademik Universitas Jenderal Soedirman. Untuk penggunaan komersial, silakan hubungi pihak universitas.*
