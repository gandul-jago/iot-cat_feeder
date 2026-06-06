#define BLYNK_TEMPLATE_ID ""
#define BLYNK_TEMPLATE_NAME "" 
#define BLYNK_AUTH_TOKEN ""

#include <WiFi.h>
#include <BlynkSimpleEsp32.h>
#include <ESP32Servo.h>
#include <LiquidCrystal_I2C.h>

// WiFi
char ssid[] = "NAMA_WIFI";
char pass[] = "PASSWORD_WIFI";

// Pin
#define TRIG_PIN 5
#define ECHO_PIN 18
#define SERVO_PIN 19
#define LED_MERAH 25
#define LED_HIJAU 26

Servo pintuMakanan;
LiquidCrystal_I2C lcd(0x27, 16, 2);

long duration;
float distance;

String statusPakan = "";

void setup() {

  Serial.begin(115200);

  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  pinMode(LED_MERAH, OUTPUT);
  pinMode(LED_HIJAU, OUTPUT);

  pintuMakanan.attach(SERVO_PIN);
  pintuMakanan.write(0);

  lcd.init();
  lcd.backlight();

  lcd.setCursor(0,0);
  lcd.print("Connecting...");

  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);

  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("WiFi Connected");

  delay(2000);
  lcd.clear();
}

// Tombol Blynk V0
BLYNK_WRITE(V0)
{
  int tombol = param.asInt();

  if(tombol == 1)
  {
    Serial.println("Mengisi Pakan");

    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Mengisi Pakan");

    pintuMakanan.write(90);

    delay(5000);

    pintuMakanan.write(0);

    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Pakan Terisi");

    Serial.println("Pakan Terisi");
  }
}

void loop() {

  Blynk.run();

  // Baca HC-SR04
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);

  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);

  digitalWrite(TRIG_PIN, LOW);

  duration = pulseIn(ECHO_PIN, HIGH);

  distance = duration * 0.034 / 2;

  Serial.print("Jarak: ");
  Serial.print(distance);
  Serial.println(" cm");

  if(distance > 15)
  {
    statusPakan = "HABIS";

    digitalWrite(LED_MERAH, HIGH);
    digitalWrite(LED_HIJAU, LOW);

    lcd.setCursor(0,0);
    lcd.print("Pakan Habis  ");

    lcd.setCursor(0,1);
    lcd.print("Isi dari App ");

    Blynk.virtualWrite(V1, "Pakan Habis");

    Serial.println("STATUS: HABIS");
  }
  else
  {
    statusPakan = "TERISI";

    digitalWrite(LED_MERAH, LOW);
    digitalWrite(LED_HIJAU, HIGH);

    lcd.setCursor(0,0);
    lcd.print("Pakan Terisi ");

    lcd.setCursor(0,1);
    lcd.print("Siap Makan   ");

    Blynk.virtualWrite(V1, "Pakan Terisi");

    Serial.println("STATUS: TERISI");
  }

  delay(1000);
}