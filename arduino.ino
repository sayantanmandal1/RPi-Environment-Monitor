#include <SPI.h>
#include <LoRa.h>
#include <DHT.h>

#define ss 10
#define rst 9
#define dio0 2

#define ledTransmit 5  
#define ledAlert 7  

#define DHTPIN 6
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

const int soilPin = A0;

float tempLow = 15, tempHigh = 35;
float humLow = 30, humHigh = 80;
float moistLow = 300, moistHigh = 1200;

unsigned long lastLoRaSend = 0;
const unsigned long loraInterval = 15000; // 15 seconds

void setup() {
  Serial.begin(115200);
  dht.begin();

  pinMode(ledTransmit, OUTPUT);
  pinMode(ledAlert, OUTPUT);
  digitalWrite(ledTransmit, LOW);
  digitalWrite(ledAlert, LOW);

  LoRa.setPins(ss, rst, dio0);
  if (!LoRa.begin(433E6)) {
    Serial.println("LoRa init failed!");
    while (1);
  }

  LoRa.setSyncWord(0x22);  // Custom sync word
  Serial.println("LoRa Initialized");
}

void loop() {
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();
  int soilMoisture = analogRead(soilPin);

  if (isnan(temperature) || isnan(humidity)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  String data = "T:" + String(temperature) + ",H:" + String(humidity) + ",M:" + String(soilMoisture);

  if (millis() - lastLoRaSend > loraInterval) {
    LoRa.beginPacket();
    LoRa.print(data);
    LoRa.endPacket();

    blinkLED(ledTransmit, 2);
    Serial.println("Sent: " + data);

    lastLoRaSend = millis();
  }

  if (isUnhealthy(temperature, humidity, soilMoisture)) {
    digitalWrite(ledAlert, HIGH);
  } else {
    digitalWrite(ledAlert, LOW);
  }

  if (Serial.available()) {
    String input = Serial.readStringUntil('\n');
    input.trim();
    parseCommand(input);
  }

  delay(3000); // Smooth loop
}

bool isUnhealthy(float t, float h, int m) {
  return t < tempLow || t > tempHigh || h < humLow || h > humHigh || m < moistLow || m > moistHigh;
}

void blinkLED(int pin, int times) {
  for (int i = 0; i < times; i++) {
    digitalWrite(pin, HIGH);
    delay(200);
    digitalWrite(pin, LOW);
    delay(200);
  }
}

void parseCommand(String input) {
  if (input == "LED:ON") {
    digitalWrite(ledAlert, HIGH);
  } else if (input == "LED:OFF") {
    digitalWrite(ledAlert, LOW);
  } else if (input.startsWith("THRESH:")) {
    sscanf(input.c_str(), "THRESH:%f,%f,%f,%f,%f,%f",
           &tempLow, &tempHigh, &humLow, &humHigh, &moistLow, &moistHigh);
  }
}

