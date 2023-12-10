#include <SoftwareSerial.h>

#define LED_PIN 7
SoftwareSerial Bluetooth(0, 1);  // Arduino(RX, TX) - Bluetooth (TX, RX)

String dataIn = "";

void setup() {
  Serial.begin(38400);
  Serial.println("Serial Inicializado");	
  Bluetooth.begin(38400);  // Default baud rate of the Bluetooth module

  pinMode(LED_PIN, OUTPUT);
}

void loop() {
  if (Bluetooth.available() > 0) {
    dataIn = Bluetooth.readString();
    Serial.print("Bluetooth Data: ");
    Serial.println(dataIn);
    delay(20);
    if (dataIn.startsWith("1")) {
      delay(10);
      Serial.println("Encendiendo LED");	
      digitalWrite(LED_PIN, HIGH);


    } else if (dataIn.startsWith("0")) {
      delay(10);
      Serial.println("Apagando LED");	
      digitalWrite(LED_PIN, LOW);
    }
  }

  delay(20);
}