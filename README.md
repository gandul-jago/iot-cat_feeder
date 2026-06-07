# IoT Cat Feeder

Sistem dispenser pakan kucing berbasis ESP32 yang dapat dipantau dan dikendalikan melalui Blynk IoT. Sistem menggunakan dua sensor ultrasonik HC-SR04 untuk memonitor kondisi mangkuk dan stok pakan pada tabung penyimpanan.

## Simulasi Wokwi

https://wokwi.com/projects/466122841372134401

## Fitur

* Monitoring kondisi pakan pada mangkuk menggunakan HC-SR04
* Monitoring stok pakan pada tabung menggunakan HC-SR04
* Menampilkan status pada LCD 16x2 I2C
* Kontrol pengisian pakan melalui aplikasi Blynk
* Servo membuka dan menutup pintu pakan
* Indikator LED status sistem
* Validasi stok habis sehingga pakan tidak dapat dikeluarkan saat tabung kosong
* Monitoring data secara real-time melalui Blynk

## Komponen

* ESP32 Dev Module
* 2x HC-SR04 Ultrasonic Sensor
* Servo Motor SG90
* LCD 16x2 I2C
* LED Merah
* LED Hijau
* Blynk IoT Platform

## Konfigurasi Pin

| Komponen             | GPIO |
| -------------------- | ---- |
| HC-SR04 Mangkuk TRIG | 5    |
| HC-SR04 Mangkuk ECHO | 18   |
| HC-SR04 Tabung TRIG  | 16   |
| HC-SR04 Tabung ECHO  | 17   |
| Servo Motor          | 19   |
| LED Merah            | 25   |
| LED Hijau            | 26   |
| LCD SDA              | 21   |
| LCD SCL              | 22   |

## Logika Sistem

### Parameter Sistem

| Parameter | Nilai |
|------------|--------|
| Tinggi Tabung Pakan | 30 cm |
| Batas Mangkuk Kosong | > 10 cm |
| Sudut Servo Tertutup | 0° |
| Sudut Servo Terbuka | 90° |
| Durasi Pengisian | 5 detik |
| Interval Pembacaan Sensor | 2 detik |

//akan direvisi sesuai kondisi asli

  
### Status Tabung

| Persentase Stok | Status  |
| --------------- | ------- |
| ≥ 75%           | PENUH   |
| 40% - 74%       | SEDANG  |
| 10% - 39%       | MENIPIS |
| < 10%           | HABIS   |

### Validasi Pengisian

Ketika status tabung berubah menjadi HABIS:

* Servo tidak dapat dibuka
* LCD menampilkan pesan:

  * TABUNG HABIS
  * TIDAK BISA ISI
* Pengguna harus mengisi ulang tabung sebelum dispenser dapat digunakan kembali

## Indikator LED

| Kondisi Mangkuk | LED Merah | LED Hijau |
| --------------- | --------- | --------- |
| KOSONG          | ON        | OFF       |
| TERISI          | OFF       | ON        |

## Dashboard Blynk

| Virtual Pin | Fungsi               |
| ----------- | -------------------- |
| V0          | Tombol Isi Pakan     |
| V1          | Status Mangkuk       |
| V2          | Status Tabung        |
| V3          | Persentase Stok      |
| V4          | Jarak Sensor Mangkuk |
| V5          | Jarak Sensor Tabung  |

## Cara Kerja

1. Sensor HC-SR04 pada mangkuk membaca kondisi pakan.
2. Sensor HC-SR04 pada tabung membaca ketinggian stok pakan.
3. Sistem menghitung persentase stok tabung.
4. Status ditampilkan pada LCD dan aplikasi Blynk.
5. Pengguna menekan tombol "Isi Pakan" pada Blynk.
6. Jika stok tersedia, servo membuka pintu selama 5 detik.
7. Pakan turun ke mangkuk.
8. Servo menutup kembali secara otomatis.
9. Jika stok habis, proses pengisian dibatalkan dan sistem menampilkan peringatan.

## Platform dan Library

### Platform

* ESP32
* PlatformIO
* Arduino Framework
* Blynk IoT

### Library

* BlynkSimpleEsp32
* ESP32Servo
* LiquidCrystal_I2C
* WiFi

## Author

Gandul
