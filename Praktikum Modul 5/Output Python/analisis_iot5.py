
import csv
import json
import os
from datetime import datetime

import pandas as pd
import matplotlib.pyplot as plt
import paho.mqtt.client as mqtt

# ==========================================
# KONFIGURASI MQTT
# ==========================================
BROKER = "broker.hivemq.com"
PORT = 1883

# HARUS SAMA dengan topic pada program ESP32.
TOPIC = "praktikum5/ardhis/dummy/data"

# ==========================================
# NAMA FILE HASIL
# ==========================================
FILE_CSV = "data_iot5.csv"
FILE_ANALISIS = "hasil_analisis.csv"
FILE_GRAFIK = "grafik_iot5.png"

# Jumlah pesan yang diterima selama sesi ini.
jumlah_pesan = 0

# ==========================================
# SIAPKAN FILE CSV
# ==========================================
if not os.path.exists(FILE_CSV) or os.path.getsize(FILE_CSV) == 0:
    with open(FILE_CSV, "w", newline="") as file:
        writer = csv.writer(file)
        writer.writerow([
            "waktu",
            "nomor",
            "suhu",
            "kelembapan",
            "suhu_filter",
            "kelembapan_filter"
        ])

print("===================================")
print(" PRAKTIKUM IOT 5 - ANALISIS DATA")
print("===================================")
print("Broker :", BROKER)
print("Port   :", PORT)
print("Topic  :", TOPIC)
print("CSV    :", FILE_CSV)


# ==========================================
# CALLBACK KONEKSI MQTT
# Dipanggil ketika status koneksi berubah.
# ==========================================
def on_connect(client, userdata, flags, reason_code, properties):
    if reason_code == 0:
        print("\n[MQTT] Berhasil terhubung ke broker.")

        # Subscribe agar Python menerima data ESP32.
        client.subscribe(TOPIC)

        print("[MQTT] Subscribe topic:", TOPIC)
        print("[MQTT] Menunggu data ESP32...\n")
    else:
        print("[MQTT] Koneksi gagal:", reason_code)


# ==========================================
# CALLBACK PESAN MQTT
# Dipanggil setiap ada pesan pada topic.
# ==========================================
def on_message(client, userdata, msg):
    global jumlah_pesan

    try:
        # Ubah byte payload menjadi teks JSON.
        teks = msg.payload.decode("utf-8")

        # Ubah JSON menjadi dictionary Python.
        data = json.loads(teks)

        # Periksa dan konversi nilai yang diperlukan.
        nomor = int(data["nomor"])
        suhu = float(data["suhu"])
        kelembapan = float(data["kelembapan"])
        suhu_filter = float(data["suhu_filter"])
        kelembapan_filter = float(data["kelembapan_filter"])

        # Abaikan nilai non-finite seperti NaN dan infinity.
        nilai = [
            suhu, kelembapan, suhu_filter, kelembapan_filter
        ]

        if not all(pd.notna(v) and abs(v) != float("inf")
                   for v in nilai):
            print("[DATA] Nilai tidak valid, dilewati.")
            return

        waktu = datetime.now().astimezone().isoformat(
            timespec="seconds"
        )

        # Simpan pesan ke CSV secara langsung.
        with open(FILE_CSV, "a", newline="") as file:
            writer = csv.writer(file)
            writer.writerow([
                waktu,
                nomor,
                suhu,
                kelembapan,
                suhu_filter,
                kelembapan_filter
            ])

        jumlah_pesan += 1

        print(
            f"[DATA {nomor:03d}] "
            f"Suhu={suhu:.2f} C | "
            f"Filter={suhu_filter:.2f} C | "
            f"Kelembapan={kelembapan:.2f}%"
        )

    except (ValueError, KeyError, json.JSONDecodeError) as error:
        print("[DATA] Pesan tidak valid:", error)


# ==========================================
# ANALISIS DATA SETELAH PENGUMPULAN
# ==========================================
def analisis_data():
    print("\n===================================")
    print(" ANALISIS DATA")
    print("===================================")

    if not os.path.exists(FILE_CSV):
        print("File CSV belum tersedia.")
        return

    df = pd.read_csv(FILE_CSV)

    # Hanya analisis data pada sesi ini.
    # Data sesi sebelumnya tetap tersimpan di CSV.
    if jumlah_pesan > 0:
        df = df.tail(jumlah_pesan).copy()

    if len(df) < 2:
        print("Data belum cukup untuk analisis.")
        return

    kolom = [
        "suhu",
        "kelembapan",
        "suhu_filter",
        "kelembapan_filter"
    ]

    # Statistik deskriptif.
    print("\n--- Statistik deskriptif ---")
    print(
        df[kolom]
        .describe()
        .round(2)
        .to_string()
    )

    # ======================================
    # DETEKSI ANOMALI DENGAN Z-SCORE
    # ======================================
    rata_suhu = df["suhu"].mean()
    std_suhu = df["suhu"].std(ddof=0)

    if std_suhu > 0:
        df["z_score_suhu"] = (
            df["suhu"] - rata_suhu
        ) / std_suhu

        df["anomali_suhu"] = (
            df["z_score_suhu"].abs() > 2
        )
    else:
        df["z_score_suhu"] = 0.0
        df["anomali_suhu"] = False

    print("\n--- Deteksi anomali suhu ---")
    print(f"Rata-rata suhu : {rata_suhu:.2f} C")
    print(f"Simpangan baku : {std_suhu:.2f}")
    print(
        "Jumlah anomali:",
        int(df["anomali_suhu"].sum())
    )

    anomali = df[df["anomali_suhu"]]

    if len(anomali) > 0:
        print("\nData yang terdeteksi sebagai anomali:")
        print(
            anomali[
                ["nomor", "suhu", "z_score_suhu"]
            ].to_string(index=False)
        )
    else:
        print("Tidak ditemukan anomali dengan ambang |z| > 2.")

    # Simpan hasil analisis ke file terpisah.
    df.to_csv(FILE_ANALISIS, index=False)
    print("\nHasil analisis disimpan:", FILE_ANALISIS)

    # ======================================
    # VISUALISASI SUHU
    # ======================================
    plt.figure(figsize=(11, 5))

    plt.plot(
        df["nomor"],
        df["suhu"],
        label="Suhu mentah",
        marker="."
    )

    plt.plot(
        df["nomor"],
        df["suhu_filter"],
        label="Moving average",
        marker="."
    )

    if anomali.shape[0] > 0:
        plt.scatter(
            anomali["nomor"],
            anomali["suhu"],
            label="Anomali (|z| > 2)",
            marker="x",
            s=90
        )

    plt.xlabel("Nomor data")
    plt.ylabel("Suhu (derajat C)")
    plt.title("Perbandingan Suhu Mentah dan Moving Average")
    plt.legend()
    plt.grid(True)
    plt.tight_layout()
    plt.savefig(FILE_GRAFIK, dpi=150)
    plt.close()

    print("Grafik disimpan:", FILE_GRAFIK)

    # ======================================
    # VISUALISASI KELEMBAPAN
    # ======================================
    plt.figure(figsize=(11, 5))

    plt.plot(
        df["nomor"],
        df["kelembapan"],
        label="Kelembapan mentah",
        marker="."
    )

    plt.plot(
        df["nomor"],
        df["kelembapan_filter"],
        label="Kelembapan moving average",
        marker="."
    )

    plt.xlabel("Nomor data")
    plt.ylabel("Kelembapan (%)")
    plt.title("Perbandingan Kelembapan Mentah dan Filter")
    plt.legend()
    plt.grid(True)
    plt.tight_layout()
    plt.savefig("grafik_kelembapan_iot5.png", dpi=150)
    plt.close()

    print("Grafik kelembapan disimpan: grafik_kelembapan_iot5.png")


# ==========================================
# PROGRAM UTAMA
# ==========================================
client = mqtt.Client(
    callback_api_version=mqtt.CallbackAPIVersion.VERSION2,
    client_id="python-iot5-ardhis"
)

client.on_connect = on_connect
client.on_message = on_message

try:
    print("\nMenghubungkan ke broker...")
    client.connect(BROKER, PORT, keepalive=60)
    client.loop_forever()

except KeyboardInterrupt:
    print("\nProgram dihentikan oleh pengguna.")

except OSError as error:
    print("\nKoneksi gagal:", error)
    print("Periksa internet, broker, dan port MQTT.")

finally:
    client.disconnect()
    client.loop_stop()

    if jumlah_pesan > 0:
        analisis_data()
    else:
        print("Belum ada data yang diterima.")
