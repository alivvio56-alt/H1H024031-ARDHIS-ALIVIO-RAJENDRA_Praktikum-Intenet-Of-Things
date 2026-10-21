# Modul 5: Pengolahan dan Analisis Data IoT

Dokumen ini berisi ringkasan singkat pelaksanaan praktikum **Modul 5: Pengolahan dan Analisis Data IoT** beserta modifikasi kode untuk **Percobaan 5A (Edge Processing)** dan **Percobaan 5B (Server Processing)**.

---

## 1. Ringkasan Praktikum

Praktikum ini bertujuan untuk menerapkan dua pendekatan utama dalam pengolahan dan analisis data sensor pada sistem IoT:
1. **Edge Processing (Percobaan 5A):** Pemrosesan data dilakukan langsung di sisi perangkat mikroprosesor (ESP8266/ESP32). Algoritma *Moving Average* ($N=5$) digunakan untuk menyaring *noise* pembacaan sensor sebelum dikirimkan atau ditayangkan.
2. **Server Processing (Percobaan 5B):** Pemrosesan data terpusat dilakukan di sisi komputer/server menggunakan Python. Data dikirimkan dari ESP via MQTT dalam format JSON, kemudian disimpan ke file `data_suhu.csv`, dianalisis menggunakan *Z-score* ($|Z| > 2$) dengan library Pandas untuk deteksi anomali, serta visualisasi grafis menggunakan Matplotlib.

---

## 2. Modifikasi Kode Percobaan 5A (Edge Processing)

### Ringkasan Modifikasi
Menambahkan fungsi perhitungan **Simpangan Baku (*Standard Deviation*)** secara *real-time* di dalam mikroprosesor berdasarkan data yang tersimpan dalam *buffer* moving average.

### Kode Program (Arduino C++)
```cpp
#include <Arduino.h>
#include <math.h>

const int N = 5;
float buffer[N];
int indeks = 0;
int jumlah = 0;

float suhuMin = 1000.0;
float suhuMax = -1000.0;

float bacaSuhuMock() {
    static unsigned long jumlahBaca = 0;
    jumlahBaca++;
    float suhuDasar = 28.0 + 2.0 * sin(jumlahBaca * 0.08);
    float noise = random(-5, 6) * 0.1;
    float suhu = suhuDasar + noise;
    if (jumlahBaca % 25 == 0) suhu += 8.0;
    return suhu;
}

float hitungMovingAverage(float nilaiBaru) {
  buffer[indeks] = nilaiBaru;
  indeks = (indeks + 1) % N;
  if (jumlah < N) jumlah++;

  float total = 0;
  for (int i = 0; i < jumlah; i++) {
    total += buffer[i];
  }
  return total / jumlah;
}

// === FUNGSI MODIFIKASI: SIMPANGAN BAKU ===
float hitungSimpanganBaku(float mean) {
  if (jumlah == 0) return 0.0;

  float jumlahKuadratSelisih = 0.0;
  for (int i = 0; i < jumlah; i++) {
    float selisih = buffer[i] - mean;
    jumlahKuadratSelisih += selisih * selisih;
  }

  float varians = jumlahKuadratSelisih / jumlah;
  return sqrt(varians);
}

void setup() {
  Serial.begin(115200);
  randomSeed(analogRead(0));
}

void loop() {
  float suhu = bacaSuhuMock();
  float suhuFilter = hitungMovingAverage(suhu);
  float stdDev = hitungSimpanganBaku(suhuFilter);

  if (suhu < suhuMin) suhuMin = suhu;
  if (suhu > suhuMax) suhuMax = suhu;

  Serial.print("Mentah: "); Serial.print(suhu);
  Serial.print(" | Mean: "); Serial.print(suhuFilter);
  Serial.print(" | StdDev: "); Serial.print(stdDev);
  Serial.print(" | Min: "); Serial.print(suhuMin);
  Serial.print(" | Max: "); Serial.println(suhuMax);

  delay(2000);
}
```

### Penjelasan Singkat Kode Tambahan 5A
* **`#include <math.h>`**: Memuat fungsi `sqrt()` untuk perhitungan akar kuadrat.
* **`float hitungSimpanganBaku(float mean)`**: Mengambil nilai rata-rata *moving average* saat ini, lalu mengulang seluruh isi *buffer* untuk menghitung selisih kuadrat $\sum (x_i - \mu)^2$.
* **`float varians = jumlahKuadratSelisih / jumlah`**: Menghitung varians sampel pada *buffer*.
* **`return sqrt(varians)`**: Menghitung akar kuadrat dari varians untuk mendapatkan nilai simpangan baku.

---

## 3. Modifikasi Kode Percobaan 5B (Server Processing)

### Ringkasan Modifikasi
Menerapkan filter **Moving Average ($W=5$)** pada data CSV di Python sebelum dilakukan analisis *Z-score*, serta membandingkan jumlah anomali terdeteksi antara data mentah dan data hasil filter.

### Kode Program (Python)
```python
import json
import time
import paho.mqtt.client as mqtt
import pandas as pd
import matplotlib.pyplot as plt

BROKER = "broker.hivemq.com"
PORT = 1883
TOPIC = "unsoed/tk245004/kelompokAnda/data"
JUMLAH_SAMPEL = 30

data = []

def on_connect(client, userdata, flags, reason_code, properties):
    client.subscribe(TOPIC)

def on_message(client, userdata, msg):
    try:
        payload = json.loads(msg.payload.decode())
        suhu = float(payload["suhu"])
    except (ValueError, KeyError):
        return

    data.append({"waktu": time.strftime("%Y-%m-%d %H:%M:%S"), "suhu": suhu})
    if len(data) >= JUMLAH_SAMPEL:
        client.disconnect()

# 1. Akuisisi Data
client = mqtt.Client(mqtt.CallbackAPIVersion.VERSION2)
client.on_connect = on_connect
client.on_message = on_message
client.connect(BROKER, PORT)
client.loop_forever()

# 2. Simpan CSV
df = pd.DataFrame(data)
df.to_csv("data_suhu.csv", index=False)

# === MODIFIKASI: ANALISIS & MOVING AVERAGE ===
# A. Z-Score Tanpa Moving Average
mean_tanpa_ma = df["suhu"].mean()
std_tanpa_ma = df["suhu"].std()
df["zscore_tanpa_ma"] = (df["suhu"] - mean_tanpa_ma) / std_tanpa_ma
df["anomali_tanpa_ma"] = df["zscore_tanpa_ma"].abs() > 2

# B. Penerapan Moving Average (Window = 5)
df["suhu_ma"] = df["suhu"].rolling(window=5, min_periods=1).mean()

# C. Z-Score Dengan Moving Average
mean_dengan_ma = df["suhu_ma"].mean()
std_dengan_ma = df["suhu_ma"].std()
df["zscore_dengan_ma"] = (df["suhu_ma"] - mean_dengan_ma) / std_dengan_ma
df["anomali_dengan_ma"] = df["zscore_dengan_ma"].abs() > 2

print(f"Anomali Tanpa MA  : {df['anomali_tanpa_ma'].sum()} data")
print(f"Anomali Dengan MA : {df['anomali_dengan_ma'].sum()} data")

# 3. Visualisasi Grafik
plt.figure(figsize=(10, 4))
plt.plot(df.index, df["suhu"], label="Suhu Mentah", color="lightgray", linestyle="--")
plt.plot(df.index, df["suhu_ma"], label="Suhu MA (W=5)", color="blue")
plt.scatter(df[df["anomali_tanpa_ma"]].index, df[df["anomali_tanpa_ma"]]["suhu"], color="red", label="Anomali (Tanpa MA)")
plt.title("Perbandingan Deteksi Anomali Sebelum dan Sesudah Moving Average")
plt.legend()
plt.tight_layout()
plt.savefig("grafik_suhu.png")
plt.show()
```

### Penjelasan Singkat Kode Tambahan 5B
* **`df["suhu"].rolling(window=5, min_periods=1).mean()`**: Membuat jendela geser berukuran 5 data dari kolom `suhu` CSV dan menghitung rata-ratanya ke kolom `suhu_ma`.
* **Perbandingan Z-Score**:
  * **Tanpa MA**: Terdeteksi **1 anomali** pada lonjakan ekstrem ($35.22^\circ\text{C}$, $Z = +3.66$).
  * **Dengan MA**: Terdeteksi **0 anomali** karena lonjakan tunggal teredam setelah dirata-ratakan bersama data normal sekitarnya.
