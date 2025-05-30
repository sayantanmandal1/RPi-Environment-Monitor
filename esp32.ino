#include <HardwareSerial.h>
#include <SPI.h>
#include <LoRa.h>
#include <WiFi.h>
#include <HTTPClient.h>

#define ss 5
#define rst 14
#define dio0 2
#define ledAnomaly 25     // Anomaly Detection LED
#define ledThingSpeak 26  // ThingSpeak Upload LED
#define ledLoRa 27        // LoRa Reception LED

const char* ssid = "SAYANTAN 1117";
const char* password = "12345678";
const char* thingspeakUrl = "http://api.thingspeak.com/update";
const char* apiKey = "ROABR0X5J6IF581U";

// Threshold values for anomaly detection (from original Arduino code)
const float TEMP_HIGH_THRESHOLD = 40.0;    // High temp threshold (°C)
const float TEMP_LOW_THRESHOLD = 5.0;      // Low temp threshold (°C)
const float HUMIDITY_HIGH_THRESHOLD = 80.0; // High humidity threshold (%)
const float HUMIDITY_LOW_THRESHOLD = 20.0;  // Low humidity threshold (%)
const int SOIL_MOISTURE_HIGH_THRESHOLD = 700; // High soil moisture threshold (raw value)
const int SOIL_MOISTURE_LOW_THRESHOLD = 300;  // Low soil moisture threshold (raw value)

void setup() {
    Serial.begin(115200);

    pinMode(ledAnomaly, OUTPUT);
    pinMode(ledThingSpeak, OUTPUT);
    pinMode(ledLoRa, OUTPUT);

    digitalWrite(ledAnomaly, LOW);
    digitalWrite(ledThingSpeak, LOW);
    digitalWrite(ledLoRa, LOW);

    connectWiFi();
    initLoRa();
}

void loop() {
    if (LoRa.parsePacket()) {
        String receivedData = LoRa.readString();
        receivedData.trim();
        Serial.println("Received: " + receivedData);

        // Blink LoRa reception LED
        digitalWrite(ledLoRa, HIGH);
        delay(100);
        digitalWrite(ledLoRa, LOW);

        float temperature, humidity, soilMoisture;
        if (parseSensorData(receivedData, temperature, humidity, soilMoisture)) {
            detectAnomaly(temperature, humidity, soilMoisture); // Check for anomalies
            sendToThingSpeak(temperature, humidity, soilMoisture);
        }
    }
}

void connectWiFi() {
    Serial.print("Connecting to WiFi...");
    WiFi.begin(ssid, password);
    int attempts = 0;

    while (WiFi.status() != WL_CONNECTED && attempts < 20) {
        delay(500);
        Serial.print(".");
        attempts++;
    }

    if (WiFi.status() == WL_CONNECTED) {
        Serial.println("\nWiFi Connected!");
    } else {
        Serial.println("\nWiFi Connection Failed! Check Credentials.");
    }
}

void initLoRa() {
    LoRa.setPins(ss, rst, dio0);
    int attempts = 0;

    while (!LoRa.begin(433E6) && attempts < 10) {
        Serial.println("LoRa init failed. Retrying...");
        delay(500);
        attempts++;
    }

    if (LoRa.begin(433E6)) {
        LoRa.setSyncWord(0x22);
        Serial.println("LoRa Initialized!");
    } else {
        Serial.println("LoRa initialization failed. Check wiring!");
    }
}

bool parseSensorData(String data, float &temp, float &hum, float &moisture) {
    int tIndex = data.indexOf("T:");
    int hIndex = data.indexOf("H:");
    int mIndex = data.indexOf("M:");

    if (tIndex == -1 || hIndex == -1 || mIndex == -1) return false;

    temp = data.substring(tIndex + 2, data.indexOf(",", tIndex)).toFloat();
    hum = data.substring(hIndex + 2, data.indexOf(",", hIndex)).toFloat();
    moisture = data.substring(mIndex + 2).toFloat();

    Serial.printf("Parsed Data -> Temp: %.2f, Hum: %.2f, Moisture: %.2f\n", temp, hum, moisture);
    return true;
}

void detectAnomaly(float temp, float hum, float moisture) {
    // Anomaly detection for temperature
    if (temp > TEMP_HIGH_THRESHOLD || temp < TEMP_LOW_THRESHOLD) {
        Serial.println("Temperature Anomaly Detected!");
        digitalWrite(ledAnomaly, HIGH);
        delay(500);  // Blink LED for anomaly
        digitalWrite(ledAnomaly, LOW);
    }

    // Anomaly detection for humidity
    if (hum > HUMIDITY_HIGH_THRESHOLD || hum < HUMIDITY_LOW_THRESHOLD) {
        Serial.println("Humidity Anomaly Detected!");
        digitalWrite(ledAnomaly, HIGH);
        delay(500);  // Blink LED for anomaly
        digitalWrite(ledAnomaly, LOW);
    }

    // Anomaly detection for soil moisture
    if (moisture > SOIL_MOISTURE_HIGH_THRESHOLD || moisture < SOIL_MOISTURE_LOW_THRESHOLD) {
        Serial.println("Soil Moisture Anomaly Detected!");
        digitalWrite(ledAnomaly, HIGH);
        delay(500);  // Blink LED for anomaly
        digitalWrite(ledAnomaly, LOW);
    }
}

void sendToThingSpeak(float temp, float hum, float moisture) {
    if (WiFi.status() != WL_CONNECTED) {
        Serial.println("WiFi Disconnected! Cannot send data.");
        return;
    }

    HTTPClient http;
    String url = String(thingspeakUrl) + "?api_key=" + apiKey +
                 "&field1=" + String(temp) + "&field2=" + String(hum) +
                 "&field3=" + String(moisture);
    
    http.begin(url);
    int httpCode = http.GET();

    if (httpCode > 0) {
        Serial.println("Data sent to ThingSpeak!");
        digitalWrite(ledThingSpeak, HIGH);
        delay(100);
        digitalWrite(ledThingSpeak, LOW);
    } else {
        Serial.println("Error sending data.");
    }

    http.end();
}
