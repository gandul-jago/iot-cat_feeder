# IoT Cat Feeder

Sistem pemberi pakan kucing otomatis berbasis ESP32 dan Blynk.

## Fitur

* Monitoring level pakan menggunakan sensor HC-SR04
* Status pakan ditampilkan pada LCD I2C
* Indikator LED merah dan hijau
* Kontrol pengisian pakan melalui aplikasi Blynk
* Servo membuka dan menutup katup pakan secara otomatis

## Komponen

* ESP32 Dev Module
* HC-SR04 Ultrasonic Sensor
* Servo Motor
* LCD 16x2 I2C
* LED Merah
* LED Hijau

## Pin Configuration

| Komponen     | GPIO |
| ------------ | ---- |
| TRIG HC-SR04 | 5    |
| ECHO HC-SR04 | 18   |
| Servo        | 19   |
| LED Merah    | 25   |
| LED Hijau    | 26   |
| SDA LCD      | 21   |
| SCL LCD      | 22   |

## Cara Kerja

1. HC-SR04 membaca level pakan.
2. Jika jarak > 15 cm maka status = Pakan Habis.
3. Status dikirim ke aplikasi Blynk.
4. Pengguna menekan tombol "Isi Pakan".
5. Servo membuka katup selama beberapa detik.
6. Pakan turun ke wadah.
7. Servo menutup kembali.

## Platform

* ESP32
* PlatformIO
* Arduino Framework
* Blynk IoT

## Author

Ashura Ken
