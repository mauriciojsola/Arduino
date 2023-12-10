// https://howtomechatronics.com/tutorials/arduino/how-to-control-ws2812b-individually-addressable-leds-using-arduino/

#include <SoftwareSerial.h>

#include "FastLED.h"

#define NUM_LEDS 45
#define LED_PIN 2
#define COLOR_ORDER GRB

CRGB leds[NUM_LEDS];

SoftwareSerial Bluetooth(53, 52);  // Arduino(RX, TX) - Bluetooth (TX, RX)

// Initial background color
int backR = 100;
int backG = 50;
int backB = 10;

// Initial reactive color
int reactiveR = 10;
int reactiveG = 50;
int reactiveB = 100;

int brightness = 50;  // Initial brightness

String dataIn = "";

void setup() {
  FastLED.addLeds<WS2812, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS);
  FastLED.setBrightness(brightness);
  Serial.begin(38400);
  Bluetooth.begin(38400);  // Default baud rate of the Bluetooth module

  for (int pinNo = 0 + 3; pinNo <= 45 + 3; pinNo++) {
    pinMode(pinNo, INPUT);
  }
}

void loop() {
  if (Bluetooth.available() > 0) {
    dataIn = Bluetooth.readString();
    delay(20);
    if (dataIn.startsWith("1")) {
      delay(10);
      String stringR = dataIn.substring(dataIn.indexOf("R") + 1, dataIn.indexOf("G"));
      reactiveR = stringR.toInt();
      String stringG = dataIn.substring(dataIn.indexOf("G") + 1, dataIn.indexOf("B"));
      reactiveG = stringG.toInt();
      String stringB = dataIn.substring(dataIn.indexOf("B") + 1, dataIn.indexOf("E"));
      reactiveB = stringB.toInt();
    } else if (dataIn.startsWith("2")) {
      String stringR = dataIn.substring(dataIn.indexOf("R") + 1, dataIn.indexOf("G"));
      backR = stringR.toInt();
      String stringG = dataIn.substring(dataIn.indexOf("G") + 1, dataIn.indexOf("B"));
      backG = stringG.toInt();
      String stringB = dataIn.substring(dataIn.indexOf("B") + 1, dataIn.indexOf("E"));
      backB = stringB.toInt();
    } else if (dataIn.startsWith("3")) {
      String stringBrightness = dataIn.substring(dataIn.indexOf("3") + 1, dataIn.length());
      brightness = stringBrightness.toInt();
      FastLED.setBrightness(brightness);
    }
  }

  for (int pinNo = 0; pinNo <= NUM_LEDS - 1; pinNo++) {
    leds[pinNo] = CRGB(backR, backG, backB);
    if (digitalRead(pinNo + 3) == LOW) {
      leds[pinNo] = CRGB(reactiveR, reactiveG, reactiveB);
    }
  }
  FastLED.show();
  delay(20);
}