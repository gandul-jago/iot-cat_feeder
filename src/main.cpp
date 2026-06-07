#define BLYNK_TEMPLATE_ID "TMPL6k_AVnqlD"
#define BLYNK_TEMPLATE_NAME "cat feeder"
#define BLYNK_AUTH_TOKEN "QNyxLXsoAIFyS0GMkDDmqxfBbMQqdIhn"

#include <WiFi.h>
#include <BlynkSimpleEsp32.h>
#include <ESP32Servo.h>
#include <LiquidCrystal_I2C.h>

char ssid[] = "NAMA_WIFI";
char pass[] = "PASSWORD_WIFI";

// ======================
// PIN
// ======================

#define TRIG_MANGKUK 5
#define ECHO_MANGKUK 18

#define TRIG_TABUNG 16
#define ECHO_TABUNG 17

#define SERVO_PIN 19

#define LED_MERAH 25
#define LED_HIJAU 26

bool sedangMengisi = false;
bool stokHabis = false;

// ======================

Servo pintuMakanan;
LiquidCrystal_I2C lcd(0x27, 16, 2);

#define PINTU_TUTUP 0
#define PINTU_BUKA 90

float tinggiTabung = 30.0;

unsigned long lastSensor = 0;

bool sedangMengisi = false;

// ======================
// BACA HC-SR04
// ======================

float bacaJarak(int trigPin, int echoPin)
{
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);

  digitalWrite(trigPin, LOW);

  long duration = pulseIn(echoPin, HIGH, 30000);

  if(duration == 0)
    return 999;

  return duration * 0.034 / 2;
}

// ======================
// SERVO
// ======================

void isiPakan()
{
  if(stokHabis)
  {
    lcd.clear();

    lcd.setCursor(0,0);
    lcd.print("TABUNG HABIS");

    lcd.setCursor(0,1);
    lcd.print("TIDAK BISA ISI");

    delay(2000);

    return;
  }

  if(sedangMengisi) return;

  sedangMengisi = true;

  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Mengisi Pakan");

  pintuMakanan.write(PINTU_BUKA);

  delay(5000);

  pintuMakanan.write(PINTU_TUTUP);

  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Selesai");

  delay(1000);

  sedangMengisi = false;
}

// ======================
// TOMBOL BLYNK
// ======================

BLYNK_WRITE(V0)
{
  if(param.asInt() == 1)
  {
    isiPakan();
  }
}

// ======================

void setup()
{
  Serial.begin(115200);

  pinMode(TRIG_MANGKUK, OUTPUT);
  pinMode(ECHO_MANGKUK, INPUT);

  pinMode(TRIG_TABUNG, OUTPUT);
  pinMode(ECHO_TABUNG, INPUT);

  pinMode(LED_MERAH, OUTPUT);
  pinMode(LED_HIJAU, OUTPUT);

  digitalWrite(LED_MERAH, LOW);
  digitalWrite(LED_HIJAU, LOW);

  pintuMakanan.attach(SERVO_PIN);
  pintuMakanan.write(PINTU_TUTUP);

  lcd.init();
  lcd.backlight();

  lcd.setCursor(0,0);
  lcd.print("Connecting...");

  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);

  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Connected");

  delay(1500);

  lcd.clear();
}

// ======================

void loop()
{
  Blynk.run();

  if(millis() - lastSensor >= 2000)
  {
    lastSensor = millis();

    float jarakMangkuk =
      bacaJarak(TRIG_MANGKUK, ECHO_MANGKUK);

    float jarakTabung =
      bacaJarak(TRIG_TABUNG, ECHO_TABUNG);

    // =====================
    // HITUNG STOK TABUNG
    // =====================

    int stokPersen =
      ((tinggiTabung - jarakTabung)
      / tinggiTabung) * 100;

    stokPersen = constrain(stokPersen, 0, 100);

    // =====================
    // STATUS TABUNG
    // =====================

    String statusTabung;

    if(stokPersen >= 75)
    {
      statusTabung = "PENUH";
    }
    else if(stokPersen >= 40)
    {
      statusTabung = "SEDANG";
    }
    else if(stokPersen >= 10)
    {
      statusTabung = "MENIPIS";
    }
    else
    {
      statusTabung = "HABIS";
    }

    stokHabis = (statusTabung == "HABIS");

    // =====================
    // STATUS MANGKUK
    // =====================

    String statusMangkuk;

    if(jarakMangkuk > 10)
    {
      statusMangkuk = "KOSONG";
    }
    else
    {
      statusMangkuk = "TERISI";
    }

    // =====================
    // LED
    // =====================

    if(statusMangkuk == "KOSONG")
    {
      digitalWrite(LED_MERAH, HIGH);
      digitalWrite(LED_HIJAU, LOW);
    }
    else
    {
      digitalWrite(LED_MERAH, LOW);
      digitalWrite(LED_HIJAU, HIGH);
    }

    // =====================
    // LCD
    // =====================

    lcd.setCursor(0,0);
    lcd.print("M:");
    lcd.print("        ");
    lcd.setCursor(2,0);
    lcd.print(statusMangkuk);

    lcd.setCursor(0,1);
    lcd.print("T:");
    lcd.print("        ");
    lcd.setCursor(2,1);
    lcd.print(statusTabung);

    // =====================
    // SERIAL
    // =====================

    Serial.println("================");
    Serial.print("Mangkuk : ");
    Serial.println(statusMangkuk);

    Serial.print("Tabung : ");
    Serial.println(statusTabung);

    Serial.print("Persen : ");
    Serial.print(stokPersen);
    Serial.println("%");

    // =====================
    // BLYNK
    // =====================

    Blynk.virtualWrite(V1, statusMangkuk);
    Blynk.virtualWrite(V2, statusTabung);
    Blynk.virtualWrite(V3, stokPersen);
    Blynk.virtualWrite(V4, jarakMangkuk);
    Blynk.virtualWrite(V5, jarakTabung);
  }
}