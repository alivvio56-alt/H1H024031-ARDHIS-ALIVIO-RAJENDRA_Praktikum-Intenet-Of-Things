# Praktikum IoT Module 2: Konfigurasi Jaringan

Nama: Ardhis Alivio  
NIM: 31  
Modul: Praktikum IoT 2 - Konfigurasi Jaringan  
Mikrokontroler: ESP8266  
Topik: Konfigurasi dan Koneksi Jaringan WiFi

---

## 1. Deskripsi Praktikum

Praktikum ini bertujuan untuk memahami penggunaan fitur konektivitas WiFi pada ESP8266. Percobaan dilakukan menggunakan tiga mode utama, yaitu Station (STA), Access Point (AP), dan AP+STA, serta pengamatan terhadap RSSI (Received Signal Strength Indicator) untuk mengetahui kekuatan sinyal WiFi.

Pada mode STA, ESP8266 bertindak sebagai client yang terhubung ke jaringan WiFi yang tersedia. Pada mode AP, ESP8266 bertindak sebagai penyedia jaringan WiFi yang dapat digunakan oleh perangkat lain. Sedangkan pada mode AP+STA, kedua fungsi tersebut dijalankan secara bersamaan.

---

## 2. Library dan Dependencies

### 2.1 Library yang Diperlukan

```cpp
#include <ESP8266WiFi.h>
```

### 2.2 ESP8266WiFi

Library ESP8266WiFi digunakan untuk mengatur komunikasi WiFi pada ESP8266.

Beberapa fungsi penting yang digunakan:

| Fungsi                       | Keterangan                                    |
| ---------------------------- | --------------------------------------------- |
| WiFi.mode()                  | Mengatur mode operasi WiFi                    |
| WiFi.begin()                 | Menghubungkan ESP8266 ke jaringan WiFi        |
| WiFi.status()                | Mengecek status koneksi WiFi                  |
| WiFi.localIP()               | Mendapatkan IP Address ESP8266 pada mode STA  |
| WiFi.macAddress()            | Mendapatkan MAC Address ESP8266               |
| WiFi.RSSI()                  | Membaca kekuatan sinyal WiFi                  |
| WiFi.softAP()                | Membuat Access Point                          |
| WiFi.softAPIP()              | Mendapatkan IP Address Access Point           |
| WiFi.softAPgetStationNum()   | Menghitung jumlah client yang terhubung ke AP |

---

## 3. Percobaan 1: Mode Station (STA)

### 3.1 Tujuan

Mengetahui proses ESP8266 terhubung ke jaringan WiFi sebagai client atau Station serta mengamati IP Address, MAC Address, RSSI, dan status koneksi.

### 3.2 Cara Kerja

Pada mode STA, ESP8266 tidak membuat jaringan sendiri. ESP8266 akan mencari dan terhubung ke jaringan WiFi berdasarkan SSID dan password yang telah diberikan.

Alur kerja:

```
ESP8266
   |
   | WiFi.begin()
   v
Router / Access Point
   |
   v
ESP8266 mendapatkan IP
   |
   v
Koneksi WiFi berhasil
```

### 3.3 Fungsi Penting

WiFi.begin(ssid, password) - Digunakan untuk memulai proses koneksi ESP8266 ke jaringan WiFi.

WiFi.status() - Digunakan untuk mengetahui apakah ESP8266 sudah terhubung atau belum.

WiFi.localIP() - Digunakan untuk mendapatkan IP Address ESP8266 setelah berhasil terhubung.

WiFi.RSSI() - Digunakan untuk mengetahui kekuatan sinyal WiFi yang diterima ESP8266.

WiFi.macAddress() - Digunakan untuk mendapatkan MAC Address ESP8266.

### 3.4 Percabangan

Contoh conditional yang digunakan:

```cpp
if (WiFi.status() == WL_CONNECTED) {
    // Koneksi berhasil
} else {
    // Koneksi gagal atau terputus
}
```

Percabangan tersebut digunakan untuk membedakan kondisi ketika ESP8266 berhasil terhubung dengan ketika ESP8266 belum atau tidak terhubung ke WiFi.

### 3.5 Hasil Pengamatan

Pada percobaan STA, ESP8266 berhasil terhubung ke jaringan WiFi dan mendapatkan IP Address. Pada saat koneksi terputus, status berubah menjadi tidak terhubung dan LED mati. Setelah koneksi kembali tersedia, ESP8266 dapat terhubung kembali dan LED menyala.

---

## 4. Percobaan 2A: Mode Access Point (AP)

### 4.1 Tujuan

Mengetahui cara ESP8266 membuat jaringan WiFi sendiri dan memungkinkan perangkat lain terhubung langsung ke ESP8266.

### 4.2 Cara Kerja

Pada mode AP, ESP8266 berfungsi sebagai penyedia jaringan WiFi.

```
           ESP8266
        Access Point
             |
     +-------+-------+
     |       |       |
    HP    Laptop   Client
```

ESP8266 membuat SSID dan password sendiri yang kemudian dapat ditemukan oleh perangkat client.

### 4.3 Fungsi Penting

WiFi.softAP(ssid, password) - Digunakan untuk membuat Access Point pada ESP8266.

WiFi.softAPIP() - Digunakan untuk mengetahui IP Address Access Point.

WiFi.softAPgetStationNum() - Digunakan untuk mengetahui jumlah perangkat yang sedang terhubung ke Access Point.

### 4.4 Hasil Pengamatan

Konfigurasi Access Point yang digunakan:

| Parameter              | Nilai            |
| ---------------------- | ---------------- |
| SSID                   | ESPTunaAsap      |
| Password               | 12345678         |
| IP Address AP          | 192.168.4.1      |
| Status AP              | Aktif            |
| SSID terdeteksi client | Ya               |
| Perangkat terhubung    | 3 perangkat      |

Berdasarkan pengamatan, ESP8266 berhasil membuat jaringan WiFi dengan SSID ESPTunaAsap. Jaringan dapat ditemukan oleh client dan berhasil digunakan oleh tiga perangkat.

---

## 5. Percobaan 2B: Pengujian Kredensial AP/STA

Pengujian dilakukan dengan memberikan kombinasi SSID dan password yang benar maupun salah.

| No | Kondisi          | SSID                       | Password  | Status Koneksi  | Output Serial Monitor | LED     |
|:--:|:-----------------|:--------------------------|:----------|:----------------|:--------------------:|:--------|
| 1  | Kredensial benar | Authentic Nasgor Tuna Asap | 12345678  | Terhubung       | Status Terhubung     | Menyala |
| 2  | Password salah   | S24                        | 111111117 | Tidak Terhubung | .....                | Mati    |
| 3  | SSID salah       | S241                       | 11111111  | Tidak Terhubung | -                    | Mati    |

### 5.1 Analisis

Ketika SSID dan password benar, ESP8266 berhasil terhubung ke jaringan dan LED menyala. Ketika password atau SSID salah, ESP8266 tidak dapat terhubung sehingga LED tetap mati.

---

## 6. Percobaan 3: Mode AP+STA

### 6.1 Tujuan

Mengetahui kemampuan ESP8266 menjalankan fungsi Station dan Access Point secara bersamaan.

### 6.2 Cara Kerja

Mode AP+STA memungkinkan ESP8266:

1. Terhubung ke WiFi rumah sebagai Station.
2. Membuat jaringan WiFi sendiri sebagai Access Point.

```
              ESP8266
             AP + STA
            /        \
           /          \
          v            v
   WiFi Rumah       ESP8266_AP
      (STA)             (AP)
       |                |
       v                v
   Internet       HP / Laptop
```

### 6.3 Fungsi Penting

WiFi.mode(WIFI_AP_STA) - Digunakan untuk mengaktifkan mode Station dan Access Point secara bersamaan.

WiFi.begin(ssidSTA, passwordSTA) - Digunakan untuk menghubungkan ESP8266 ke WiFi rumah sebagai Station.

WiFi.softAP(ssidAP, passwordAP) - Digunakan untuk membuat jaringan WiFi sendiri sebagai Access Point.

WiFi.localIP() - Menampilkan IP Address ESP8266 pada jaringan WiFi rumah.

WiFi.softAPIP() - Menampilkan IP Address Access Point yang dibuat ESP8266.

### 6.4 Conditional

Contoh pengecekan koneksi:

```cpp
if (WiFi.status() == WL_CONNECTED) {
    Serial.println("Status STA: Terhubung");
} else {
    Serial.println("Status STA: Terputus");
}
```

Percabangan digunakan untuk menentukan apakah ESP8266 masih terhubung dengan WiFi rumah atau tidak.

---

## 7. Program 1: STA dengan Auto Reconnect

### 7.1 Tujuan

Modifikasi ini bertujuan agar ESP8266 dapat mendeteksi ketika koneksi WiFi terputus dan mencoba melakukan koneksi ulang secara otomatis tanpa harus melakukan restart perangkat.

### 7.2 Kode Program

```cpp
#include <ESP8266WiFi.h>

const char* ssid = "NAMA_WIFI";
const char* password = "PASSWORD_WIFI";

const int LED_PIN = LED_BUILTIN;

void setup() {
  Serial.begin(115200);

  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, HIGH);

  WiFi.mode(WIFI_STA);

  WiFi.begin(ssid, password);

  Serial.print("Menghubungkan ke WiFi");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println();
  Serial.println("WiFi Terhubung");

  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  digitalWrite(LED_PIN, LOW);
}

void loop() {

  if (WiFi.status() != WL_CONNECTED) {

    digitalWrite(LED_PIN, HIGH);

    Serial.println("WiFi Terputus!");
    Serial.println("Mencoba reconnect...");

    WiFi.disconnect();

    WiFi.begin(ssid, password);

    unsigned long startTime = millis();

    while (WiFi.status() != WL_CONNECTED &&
           millis() - startTime < 10000) {

      delay(500);
      Serial.print(".");
    }

    Serial.println();

    if (WiFi.status() == WL_CONNECTED) {

      Serial.println("Reconnect berhasil!");

      Serial.print("IP Address: ");
      Serial.println(WiFi.localIP());

      digitalWrite(LED_PIN, LOW);
    }

  } else {

    digitalWrite(LED_PIN, LOW);
  }

  delay(1000);
}
```

### 7.3 Penjelasan Fungsi Program

WiFi.mode(WIFI_STA) - Digunakan untuk mengatur ESP8266 agar bekerja sebagai Station, yaitu client yang terhubung ke jaringan WiFi.

WiFi.begin() - Digunakan untuk memulai koneksi ESP8266 ke jaringan WiFi menggunakan SSID dan password.

WiFi.status() - Digunakan untuk mengetahui kondisi koneksi WiFi. Status WL_CONNECTED menunjukkan terhubung, selain itu menunjukkan belum terhubung atau terputus.

WiFi.disconnect() - Digunakan untuk memutus koneksi WiFi yang sedang digunakan sebelum melakukan percobaan koneksi ulang.

millis() - Digunakan untuk menghitung waktu yang telah berlalu sejak ESP8266 mulai berjalan. Pada program ini digunakan untuk memberikan batas waktu 10 detik pada proses reconnect.

WiFi.localIP() - Digunakan untuk mendapatkan IP Address ESP8266 setelah berhasil terhubung ke WiFi.

### 7.4 Penjelasan Conditional Auto Reconnect

Bagian utama modifikasi terdapat pada:

```cpp
if (WiFi.status() != WL_CONNECTED) {
```

Conditional tersebut digunakan untuk mendeteksi apakah koneksi WiFi terputus.

Jika koneksi terputus:

```
WiFi terputus
      v
WiFi.disconnect()
      v
WiFi.begin()
      v
Mencoba reconnect
      v
Berhasil?
   /       \
 Ya        Tidak
 v           v
Terhubung   Coba lagi
```

Batas waktu reconnect diberikan menggunakan:

```cpp
while (WiFi.status() != WL_CONNECTED &&
       millis() - startTime < 10000)
```

Artinya ESP8266 akan mencoba terhubung selama maksimal 10 detik pada satu proses reconnect.

---

## 8. Program 2: AP+STA

### 8.1 Tujuan

Modifikasi kedua bertujuan agar ESP8266 dapat bekerja dalam dua mode secara bersamaan, yaitu STA terhubung ke WiFi rumah dan AP membuat jaringan WiFi sendiri.

### 8.2 Kode Program

```cpp
#include <ESP8266WiFi.h>

// Konfigurasi WiFi rumah untuk mode STA
const char* ssidSTA = "NAMA_WIFI_RUMAH";
const char* passwordSTA = "PASSWORD_WIFI";

// Konfigurasi Access Point
const char* ssidAP = "ESP8266_AP";
const char* passwordAP = "12345678";

void setup() {

  Serial.begin(115200);

  delay(100);

  Serial.println();
  Serial.println("ESP8266 AP + STA");

  WiFi.mode(WIFI_AP_STA);

  // Memulai koneksi ke WiFi rumah
  WiFi.begin(ssidSTA, passwordSTA);

  Serial.print("Menghubungkan ke WiFi rumah");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println();
  Serial.println("WiFi rumah berhasil terhubung!");

  Serial.print("IP STA: ");
  Serial.println(WiFi.localIP());

  // Membuat Access Point
  WiFi.softAP(ssidAP, passwordAP);

  Serial.println("Access Point berhasil dibuat!");

  Serial.print("SSID AP: ");
  Serial.println(ssidAP);

  Serial.print("IP AP: ");
  Serial.println(WiFi.softAPIP());
}

void loop() {

  if (WiFi.status() == WL_CONNECTED) {

    Serial.println("Status STA: Terhubung");

  } else {

    Serial.println("Status STA: Terputus");
  }

  Serial.print("Jumlah perangkat AP: ");

  Serial.println(WiFi.softAPgetStationNum());

  delay(5000);
}
```

### 8.3 Penjelasan Fungsi Program

WiFi.mode(WIFI_AP_STA) - Merupakan fungsi utama pada program AP+STA. Fungsi ini membuat ESP8266 dapat menjalankan Station dan Access Point secara bersamaan.

WiFi.begin() - Digunakan agar ESP8266 terhubung ke WiFi rumah sebagai Station.

WiFi.softAP() - Digunakan untuk membuat Access Point dengan SSID dan password yang ditentukan.

WiFi.softAPIP() - Digunakan untuk mendapatkan IP Address Access Point ESP8266. Secara default biasanya 192.168.4.1

WiFi.softAPgetStationNum() - Digunakan untuk mengetahui jumlah perangkat yang sedang terhubung ke Access Point ESP8266.

---

## 9. Perbandingan Kedua Modifikasi

| Parameter                       | STA Auto Reconnect | AP+STA                 |
|---------------------------------|:------------------:|:----------------------:|
| Mode                            | Station            | Station + Access Point |
| Terhubung ke WiFi rumah         | Ya                 | Ya                     |
| Membuat WiFi sendiri            | Tidak              | Ya                     |
| Reconnect otomatis              | Ya                 | Belum                  |
| Client dapat terhubung langsung | Tidak              | Ya                     |
| Fungsi utama                    | Menjaga koneksi    | Koneksi + provisioning |

---

## 10. RSSI (Received Signal Strength Indicator)

### 10.1 Pengertian

RSSI adalah nilai yang menunjukkan kekuatan sinyal WiFi yang diterima perangkat. Nilainya dinyatakan dalam satuan dBm dan biasanya berupa angka negatif.

Secara umum:

- Nilai mendekati 0 dBm menunjukkan sinyal lebih kuat.
- Nilai semakin negatif menunjukkan sinyal semakin lemah.

Contoh:

```
-40 dBm   sinyal lebih kuat
-65 dBm   sinyal lebih lemah dibanding -40 dBm
```

### 10.2 Fungsi Pembacaan RSSI

```cpp
WiFi.RSSI();
```

Fungsi tersebut digunakan untuk membaca nilai kekuatan sinyal WiFi yang diterima ESP8266.

### 10.3 Hasil Pengamatan

Pada percobaan diperoleh nilai RSSI sebesar -3 dBm dan -65 dBm pada kondisi pengujian yang berbeda. Perubahan nilai RSSI menunjukkan adanya perbedaan kekuatan sinyal yang diterima oleh ESP8266. Semakin lemah sinyal, kemungkinan gangguan komunikasi dan ketidakstabilan koneksi akan semakin besar.

---

## 11. Penjelasan Conditional

Conditional digunakan untuk menentukan tindakan program berdasarkan kondisi tertentu.

Contoh:

```cpp
if (WiFi.status() == WL_CONNECTED) {
    Serial.println("Terhubung");
} else {
    Serial.println("Tidak Terhubung");
}
```

Jika kondisi WiFi.status() == WL_CONNECTED bernilai benar, program menampilkan "Terhubung". Jika kondisi bernilai salah, program menjalankan bagian else dan menampilkan "Tidak Terhubung".

Conditional juga dapat digunakan untuk mengendalikan LED berdasarkan status koneksi:

```cpp
if (WiFi.status() == WL_CONNECTED) {
    digitalWrite(LED_PIN, HIGH);
} else {
    digitalWrite(LED_PIN, LOW);
}
```

Ketika WiFi terhubung, LED menyala. Ketika WiFi tidak terhubung, LED mati.

---

## 12. Skenario Pengujian

### 12.1 Pengujian Auto Reconnect

Pengujian dilakukan dengan:

1. Menyalakan ESP8266.
2. Memastikan ESP8266 berhasil terhubung ke WiFi.
3. Mematikan atau memutus jaringan WiFi.
4. Mengamati Serial Monitor.
5. Menghidupkan kembali jaringan WiFi.
6. Mengamati apakah ESP8266 melakukan koneksi ulang.

Hasil yang diharapkan:

```
WiFi Terhubung
IP Address: xxx.xxx.xxx.xxx

WiFi Terputus!
Mencoba reconnect...
......

Reconnect berhasil!
IP Address: xxx.xxx.xxx.xxx
```

Dengan demikian, ESP8266 tidak perlu di-reset secara manual ketika koneksi WiFi terputus.

### 12.2 Pengujian AP+STA

Pengujian dilakukan dengan:

1. Menyalakan ESP8266.
2. ESP8266 melakukan koneksi ke WiFi rumah.
3. ESP8266 membuat Access Point.
4. Mencari SSID AP menggunakan HP/laptop.
5. Menghubungkan client ke Access Point ESP8266.
6. Mengamati IP Address STA dan AP pada Serial Monitor.

Contoh output:

```
ESP8266 AP + STA

WiFi rumah berhasil terhubung!
IP STA: 192.168.1.xxx

Access Point berhasil dibuat!
SSID AP: ESP8266_AP
IP AP: 192.168.4.1

Status STA: Terhubung
Jumlah perangkat AP: 1
```

---

## 13. Pertanyaan Praktikum

### 13.1 Uraikan hasil tugas pada praktikum yang telah dilakukan pada setiap percobaan!

Percobaan STA menunjukkan bahwa ESP8266 dapat terhubung ke jaringan WiFi sebagai client dan memperoleh IP Address. Percobaan AP menunjukkan bahwa ESP8266 dapat membuat jaringan WiFi sendiri yang dapat ditemukan dan digunakan oleh perangkat lain. Pada mode AP+STA, ESP8266 dapat terhubung ke WiFi rumah sekaligus menyediakan Access Point sendiri.

### 13.2 Bagaimana pengaruh kekuatan sinyal (RSSI) terhadap kestabilan koneksi WiFi pada perangkat IoT?

RSSI menunjukkan kekuatan sinyal WiFi yang diterima perangkat. Sinyal yang lebih kuat umumnya menghasilkan koneksi yang lebih stabil, sedangkan sinyal yang semakin lemah dapat menyebabkan komunikasi terganggu, kecepatan transfer menurun, dan koneksi lebih mudah terputus.

### 13.3 Bagaimana cara kerja ESP8266 dalam membedakan peran sebagai klien (Station) dan sebagai penyedia jaringan (Access Point)?

Pada mode Station, perangkat bertindak sebagai client yang terhubung ke router atau Access Point menggunakan SSID dan password. Pada mode Access Point, perangkat bertindak sebagai penyedia jaringan dengan membuat SSID dan password sendiri yang dapat digunakan oleh perangkat lain.

### 13.4 Bagaimana kombinasi mode Station dan Access Point (AP+STA) dapat dimanfaatkan dalam skenario nyata sistem IoT?

Mode AP+STA dapat digunakan dalam proses provisioning perangkat IoT. ESP8266 dapat membuat Access Point agar pengguna dapat terhubung dan memasukkan konfigurasi WiFi rumah. Setelah konfigurasi diperoleh, ESP8266 menggunakan mode Station untuk terhubung ke jaringan WiFi tersebut. Dengan demikian, perangkat dapat dikonfigurasi tanpa harus mengubah program secara manual.

### 13.5 Mengapa diperlukan fitur auto reconnect?

Auto reconnect diperlukan agar ESP8266 dapat kembali terhubung ke jaringan WiFi secara otomatis ketika koneksi terputus. Hal ini penting pada sistem IoT karena perangkat biasanya harus bekerja dalam waktu lama tanpa perlu di-reset secara manual.

### 13.6 Apa fungsi WiFi.status()?

WiFi.status() digunakan untuk mengetahui kondisi koneksi WiFi ESP8266. Fungsi ini digunakan dalam conditional untuk menentukan apakah ESP8266 sedang terhubung atau terputus.

### 13.7 Apa fungsi WIFI_AP_STA?

WIFI_AP_STA digunakan untuk mengaktifkan dua mode WiFi secara bersamaan, yaitu Station dan Access Point.

### 13.8 Apa keuntungan AP+STA pada perangkat IoT?

AP+STA memungkinkan perangkat tetap terhubung ke jaringan utama sebagai Station sekaligus menyediakan jaringan sendiri untuk konfigurasi atau komunikasi langsung dengan perangkat lain.

Contohnya pada proses provisioning:

```
       HP / Laptop
            |
            | Terhubung ke AP
            v
       +----------+
       | ESP8266  |
       +----------+
            |
            | STA
            v
       WiFi Rumah
            |
            v
         Internet
```

Pengguna dapat terhubung ke Access Point ESP8266 untuk memberikan konfigurasi WiFi rumah. Setelah konfigurasi selesai, ESP8266 menggunakan mode STA untuk terhubung ke jaringan rumah.

---

## 14. Skematik dan Diagram Sistem

### 14.1 Mode STA

```
       Router / WiFi Rumah
               |
               |
             WiFi
               |
               v
           ESP8266
            (STA)
```

### 14.2 Mode AP

```
             ESP8266
              (AP)
               |
       +-------+-------+
       |       |       |
      HP    Laptop   Client
```

### 14.3 Mode AP+STA

```
          WiFi Rumah
              |
              | STA
              v
          +--------+
          | ESP8266|
          +--------+
              |
              | AP
              v
       +------+------+
       |      |      |
      HP    Laptop  Client
```

---

## 15. Dokumentasi Percobaan

<img width="1280" height="722" alt="WhatsApp Image 2026-09-09 at 12 20 30 (1)" src="https://github.com/user-attachments/assets/5c37eeb9-c799-4deb-a7be-ccae5f09f2ca" />

<img width="1280" height="722" alt="WhatsApp Image 2026-09-09 at 12 20 30" src="https://github.com/user-attachments/assets/f1946393-bbe1-4ed7-a55c-1bc0cd29e21b" />



## 16. Kesimpulan

Praktikum menunjukkan bahwa ESP8266 memiliki beberapa mode operasi WiFi yang dapat digunakan sesuai kebutuhan sistem IoT. Mode STA digunakan untuk menghubungkan ESP8266 ke jaringan yang tersedia, mode AP digunakan untuk membuat jaringan sendiri, sedangkan AP+STA memungkinkan kedua fungsi berjalan secara bersamaan. Selain itu, nilai RSSI dapat digunakan untuk mengetahui kekuatan sinyal dan membantu menganalisis kestabilan koneksi WiFi pada perangkat IoT.

Modifikasi pertama berhasil menambahkan mekanisme auto reconnect sehingga ESP8266 dapat mendeteksi koneksi WiFi yang terputus dan mencoba menghubungkan kembali secara otomatis. Modifikasi kedua membuat ESP8266 mampu menjalankan AP+STA, sehingga perangkat dapat terhubung ke WiFi rumah sekaligus menyediakan Access Point sendiri. Kedua modifikasi tersebut meningkatkan fleksibilitas dan keandalan konektivitas ESP8266 dalam penerapan sistem IoT.

---

## 17. Referensi dan Sumber Pembelajaran

- ESP8266 Arduino Core Documentation
- ESP8266 WiFi Library Reference
- Datasheet ESP8266 Espressif Systems

