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
    print("Terhubung ke broker, kode:", reason_code)
    client.subscribe(TOPIC)

def on_message(client, userdata, msg):
    try:
        payload = json.loads(msg.payload.decode())
        suhu = float(payload["suhu"])
    except (ValueError, KeyError):
        return

    data.append({"waktu": time.strftime("%Y-%m-%d %H:%M:%S"), "suhu": suhu})
    print(f"[{len(data)}/{JUMLAH_SAMPEL}] Suhu diterima = {suhu}")

    if len(data) >= JUMLAH_SAMPEL:
        client.disconnect()

# -- Tahap 1: Akuisisi Data via MQTT
client = mqtt.Client(mqtt.CallbackAPIVersion.VERSION2)
client.on_connect = on_connect
client.on_message = on_message
client.connect(BROKER, PORT)
print("Mulai mengumpulkan 30 data dari MQTT...")
client.loop_forever()

# -- Tahap 2: Simpan Data Asli ke CSV
df = pd.DataFrame(data)
df.to_csv("data_suhu.csv", index=False)
print("\nData tersimpan di data_suhu.csv")

# ==============================================================================
# MODIFIKASI: Penerapan Moving Average & Perbandingan Deteksi Anomali
# ==============================================================================

# A. Analisis Z-Score TANPA Moving Average (Data Mentah)
mean_tanpa_ma = df["suhu"].mean()
std_tanpa_ma = df["suhu"].std()
df["zscore_tanpa_ma"] = (df["suhu"] - mean_tanpa_ma) / std_tanpa_ma
df["anomali_tanpa_ma"] = df["zscore_tanpa_ma"].abs() > 2

# B. Penerapan Moving Average (Jendela 5 Data) pada Data CSV
df["suhu_ma"] = df["suhu"].rolling(window=5, min_periods=1).mean()

# C. Analisis Z-Score DENGAN Moving Average
mean_dengan_ma = df["suhu_ma"].mean()
std_dengan_ma = df["suhu_ma"].std()
df["zscore_dengan_ma"] = (df["suhu_ma"] - mean_dengan_ma) / std_dengan_ma
df["anomali_dengan_ma"] = df["zscore_dengan_ma"].abs() > 2

# Output Hasil Perbandingan
print("\n--- HASIL PERBANDINGAN DETEKSI ANOMALI ---")
print(f"Jumlah Anomali Tanpa Moving Average  : {df['anomali_tanpa_ma'].sum()} data")
print(f"Jumlah Anomali Dengan Moving Average : {df['anomali_dengan_ma'].sum()} data")

# -- Tahap 4: Visualisasi Grafik Perbandingan
plt.figure(figsize=(12, 5))
plt.plot(df.index, df["suhu"], label="Suhu Mentah", color="lightgray", linestyle="--")
plt.plot(df.index, df["suhu_ma"], label="Suhu Moving Average (W=5)", color="blue", linewidth=2)

# Tandai anomali tanpa MA
anom_tanpa = df[df["anomali_tanpa_ma"]]
plt.scatter(anom_tanpa.index, anom_tanpa["suhu"], color="red", s=100, label="Anomali (Tanpa MA)", zorder=4)

# Tandai anomali dengan MA
anom_dengan = df[df["anomali_dengan_ma"]]
if not anom_dengan.empty:
    plt.scatter(anom_dengan.index, anom_dengan["suhu_ma"], color="purple", s=100, marker="x", label="Anomali (Dengan MA)", zorder=5)

plt.axhline(mean_dengan_ma, color="gray", linestyle=":", label="Rata-rata MA")
plt.xlabel("Urutan Sampel Data")
plt.ylabel("Suhu (°C)")
plt.title("Perbandingan Deteksi Anomali Suhu Sebelum dan Sesudah Moving Average")
plt.legend()
plt.tight_layout()

plt.savefig("grafik_suhu.png")
print("Grafik tersimpan sebagai grafik_suhu.png")
plt.show()