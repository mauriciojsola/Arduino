// https://howtomechatronics.com/tutorials/arduino/how-to-control-ws2812b-individually-addressable-leds-using-arduino/

#include <SoftwareSerial.h>
#include "FastLED.h"
FASTLED_USING_NAMESPACE

#define NUM_LEDS 16
#define LED_PIN 8
#define COLOR_ORDER GRB
#define FRAMES_PER_SECOND 48

CRGB leds[NUM_LEDS];

SoftwareSerial Bluetooth(0, 1);  // Arduino(RX, TX) - Bluetooth (TX, RX)

// Initial background color
byte backR = 100;
byte backG = 50;
byte backB = 10;

// Initial reactive color
byte reactiveR = 10;
byte reactiveG = 50;
byte reactiveB = 100;

int brightness = 10;  // Initial brightness

byte currentMode = 2;  // Initial mode: table background
String dataIn = "";

void setup() {
  delay(3000);  // 3 second delay for recovery

  FastLED.addLeds<WS2812, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS);
  FastLED.setBrightness(brightness);
  Serial.begin(38400);
  Bluetooth.begin(38400);  // Default baud rate of the Bluetooth module

  initialize();
  // for (int pinNo = 0 + 3; pinNo <= 45 + 3; pinNo++) {
  //   pinMode(pinNo, INPUT);
  // }
}

// DEMO: List of patterns to cycle through.  Each is defined as a separate function below.
typedef void (*SimplePatternList[])();
SimplePatternList gPatterns = { rainbow, rainbowWithGlitter, confetti, sinelon, juggle, bpm };
uint8_t gCurrentPatternNumber = 0;  // Index number of which pattern is current
uint8_t gHue = 0;                   // rotating "base color" used by many of the patterns

void loop() {
  if (Bluetooth.available() > 0) {
    dataIn = Bluetooth.readString();
    Serial.print("Received: [");
    Serial.print(dataIn);
    Serial.println("]");
    delay(20);

    if (dataIn == "" || dataIn == NULL) {
      currentMode = 2;
    } else {
      currentMode = dataIn.substring(0, 1).toInt();
    }

    if (currentMode == 2) {  // Table Background
      String stringR = dataIn.substring(dataIn.indexOf("R") + 1, dataIn.indexOf("G"));
      backR = stringR.toInt();
      String stringG = dataIn.substring(dataIn.indexOf("G") + 1, dataIn.indexOf("B"));
      backG = stringG.toInt();
      String stringB = dataIn.substring(dataIn.indexOf("B") + 1, dataIn.indexOf("E"));
      backB = stringB.toInt();

      updateTable(backR, backG, backB);

    } else if (currentMode == 3) {  // Table Brightness
      String stringBrightness = dataIn.substring(dataIn.indexOf("3") + 1, dataIn.length());
      brightness = stringBrightness.toInt();
      FastLED.setBrightness(brightness);
      FastLED.delay(1000 / FRAMES_PER_SECOND);
    }
  }

  // In mode 1 we should update the demo effects
  if (currentMode == 1) {
    delay(10);
    randomEffects();
  }
}

void initialize() {
  for (int pinNo = 0; pinNo <= NUM_LEDS - 1; pinNo++) {
    leds[pinNo] = CRGB(backR, backG, backB);
    // if (digitalRead(pinNo + 3) == LOW) {
    //   leds[pinNo] = CRGB(reactiveR, reactiveG, reactiveB);
    // }
  }
  FastLED.show();
  // insert a delay to keep the framerate modest
  FastLED.delay(1000 / FRAMES_PER_SECOND);
}

void updateTable(byte R, byte G, byte B) {
  for (int pinNo = 0; pinNo <= NUM_LEDS - 1; pinNo++) {
    leds[pinNo] = CRGB(R, G, B);
    // if (digitalRead(pinNo + 3) == LOW) {
    //   leds[pinNo] = CRGB(reactiveR, reactiveG, reactiveB);
    // }
  }
  FastLED.show();
  // insert a delay to keep the framerate modest
  FastLED.delay(1000 / FRAMES_PER_SECOND);
}

//****************************************************************************
// *** START Random effects
//****************************************************************************

#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))

void randomEffects() {
  // Call the current pattern function once, updating the 'leds' array
  gPatterns[gCurrentPatternNumber]();

  // send the 'leds' array out to the actual LED strip
  FastLED.setBrightness(brightness);
  FastLED.show();
  // insert a delay to keep the framerate modest
  FastLED.delay(1000 / FRAMES_PER_SECOND);

  // do some periodic updates
  EVERY_N_MILLISECONDS(50) {
    gHue++;
  }  // slowly cycle the "base color" through the rainbow
  EVERY_N_SECONDS(20) {
    nextPattern();
  }  // change patterns periodically
}

void nextPattern() {
  // add one to the current pattern number, and wrap around at the end
  gCurrentPatternNumber = (gCurrentPatternNumber + 1) % ARRAY_SIZE(gPatterns);
}

void rainbow() {
  // FastLED's built-in rainbow generator
  fill_rainbow(leds, NUM_LEDS, gHue, 7);
}

void rainbowWithGlitter() {
  // built-in FastLED rainbow, plus some random sparkly glitter
  rainbow();
  addGlitter(80);
}

void addGlitter(fract8 chanceOfGlitter) {
  if (random8() < chanceOfGlitter) {
    leds[random16(NUM_LEDS)] += CRGB::White;
  }
}

void confetti() {
  // random colored speckles that blink in and fade smoothly
  fadeToBlackBy(leds, NUM_LEDS, 10);
  int pos = random16(NUM_LEDS);
  leds[pos] += CHSV(gHue + random8(64), 200, 255);
}

void sinelon() {
  // a colored dot sweeping back and forth, with fading trails
  fadeToBlackBy(leds, NUM_LEDS, 20);
  int pos = beatsin16(13, 0, NUM_LEDS - 1);
  leds[pos] += CHSV(gHue, 255, 192);
}

void bpm() {
  // colored stripes pulsing at a defined Beats-Per-Minute (BPM)
  uint8_t BeatsPerMinute = 25;
  CRGBPalette16 palette = PartyColors_p;
  uint8_t beat = beatsin8(BeatsPerMinute, 64, 255);
  for (int i = 0; i < NUM_LEDS; i++) {  //9948
    leds[i] = ColorFromPalette(palette, gHue + (i * 2), beat - gHue + (i * 10));
  }
}

void juggle() {
  // eight colored dots, weaving in and out of sync with each other
  fadeToBlackBy(leds, NUM_LEDS, 20);
  uint8_t dothue = 0;
  for (int i = 0; i < 8; i++) {
    leds[beatsin16(i + 7, 0, NUM_LEDS - 1)] |= CHSV(dothue, 200, 255);
    dothue += 32;
  }
}

//****************************************************************************
// *** END Random effects
//****************************************************************************














//   if (dataIn.startsWith("1")) {
//     delay(10);
//     String stringR = dataIn.substring(dataIn.indexOf("R") + 1, dataIn.indexOf("G"));
//     reactiveR = stringR.toInt();
//     String stringG = dataIn.substring(dataIn.indexOf("G") + 1, dataIn.indexOf("B"));
//     reactiveG = stringG.toInt();
//     String stringB = dataIn.substring(dataIn.indexOf("B") + 1, dataIn.indexOf("E"));
//     reactiveB = stringB.toInt();
//   } else if (dataIn.startsWith("2")) {
//     String stringR = dataIn.substring(dataIn.indexOf("R") + 1, dataIn.indexOf("G"));
//     backR = stringR.toInt();
//     String stringG = dataIn.substring(dataIn.indexOf("G") + 1, dataIn.indexOf("B"));
//     backG = stringG.toInt();
//     String stringB = dataIn.substring(dataIn.indexOf("B") + 1, dataIn.indexOf("E"));
//     backB = stringB.toInt();
//   } else if (dataIn.startsWith("3")) {
//     String stringBrightness = dataIn.substring(dataIn.indexOf("3") + 1, dataIn.length());
//     brightness = stringBrightness.toInt();
//     FastLED.setBrightness(brightness);
//   }
// }
