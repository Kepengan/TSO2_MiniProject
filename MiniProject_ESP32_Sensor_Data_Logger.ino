#include <WiFi.h>
#include <HTTPClient.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

/* ---------- PIN DEFINITIONS ---------- */
#define SDA_PIN 21
#define SCL_PIN 22

#define TRIG_PIN 13
#define ECHO_PIN 12

#define LED_PIN 2   // ESP32 built-in LED (ACTIVE-LOW)

/* ---------- WIFI ---------- */
const char* ssid = "i.m";
const char* password = "siomdulu";

/* ---------- GOOGLE SHEET URL ---------- */
const char* serverURL =
"https://script.google.com/macros/s/AKfycbyDYsa0cPx5rri4JJRsHstzrdy-6BUwzbk6EpTjLSYcRmNcwcQbvBpi4M1mdL6rYSut/exec";

/* ---------- SENSOR ---------- */
Adafruit_BME280 bme;

/* ---------- WIFI LED VARIABLES ---------- */
unsigned long previousBlinkMillis = 0;
const unsigned long blinkInterval = 500;
bool ledState = false;

/* ---------- AVERAGING VARIABLES ---------- */
unsigned long previousReadMillis = 0;
const unsigned long readInterval = 60000; // 1 minute

int sampleCount = 0;
const int maxSamples = 30;

// Running sums
float sumTemp = 0;
float sumHum  = 0;
float sumPres = 0;
float sumDist = 0;

/* ---------- WIFI LED FUNCTION ---------- */
void wifiLEDStatus() {
  if (WiFi.status() == WL_CONNECTED) {
    unsigned long currentMillis = millis();
    if (currentMillis - previousBlinkMillis >= blinkInterval) {
      previousBlinkMillis = currentMillis;
      ledState = !ledState;
      digitalWrite(LED_PIN, !ledState); // ACTIVE-LOW
    }
  } else {
    digitalWrite(LED_PIN, LOW); // ON when NOT connected
  }
}

/* ---------- SETUP ---------- */
void setup() {
  Serial.begin(115200);

  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);

  // WiFi
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");

  while (WiFi.status() != WL_CONNECTED) {
    wifiLEDStatus();
    delay(500);
    Serial.print(".");
  }

  Serial.println("\n✅ WiFi connected");

  // I2C
  Wire.begin(SDA_PIN, SCL_PIN);

  if (!bme.begin(0x76)) {
    Serial.println("❌ BME280 not found");
    while (1);
  }

  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  Serial.println("✅ System ready");
}

/* ---------- LOOP ---------- */
void loop() {
  wifiLEDStatus();

  unsigned long currentMillis = millis();

  /* ---------- READ SENSOR EVERY 1 MINUTE ---------- */
  if (currentMillis - previousReadMillis >= readInterval) {
    previousReadMillis = currentMillis;

    // Read BME280
    float temp = bme.readTemperature();
    float hum  = bme.readHumidity();
    float pres = (bme.readPressure() / 100.0F) / 1013.25;

    // Read HC-SR04
    digitalWrite(TRIG_PIN, LOW);
    delayMicroseconds(2);
    digitalWrite(TRIG_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIG_PIN, LOW);

    long duration = pulseIn(ECHO_PIN, HIGH, 100000);
    float dist = (duration > 0) ? duration * 0.034 / 2 : 0;

    // Store running sums
    sumTemp += temp;
    sumHum  += hum;
    sumPres += pres;
    sumDist += dist;

    sampleCount++;

    Serial.print("📊 Sample ");
    Serial.print(sampleCount);
    Serial.print(" | Temp: ");
    Serial.print(temp);
    Serial.print(" °C | Hum: ");
    Serial.print(hum);
    Serial.print(" % | Pres: ");
    Serial.print(pres);
    Serial.print(" atm | Dist: ");
    Serial.print(dist);
    Serial.println(" cm");
  }

  /* ---------- SEND AVERAGE AFTER 30 MINUTES ---------- */
  if (sampleCount >= maxSamples) {

    float avgTemp = sumTemp / maxSamples;
    float avgHum  = sumHum  / maxSamples;
    float avgPres = sumPres / maxSamples;
    float avgDist = sumDist / maxSamples;

    if (WiFi.status() == WL_CONNECTED) {
      HTTPClient http;

      String url = String(serverURL) +
        "?temp=" + String(avgTemp, 2) +
        "&hum="  + String(avgHum, 2) +
        "&pres=" + String(avgPres, 4) +
        "&dist=" + String(avgDist, 2);

      http.begin(url);
      int httpCode = http.GET();

      if (httpCode > 0) {
        Serial.println("📤 30-minute AVERAGE sent to Google Sheets");
      } else {
        Serial.println("❌ Failed to send average data");
      }

      http.end();
    }

    // Reset for next cycle
    sampleCount = 0;
    sumTemp = sumHum = sumPres = sumDist = 0;
  }
}