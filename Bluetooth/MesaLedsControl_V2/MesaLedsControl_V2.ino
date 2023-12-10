// https://howtomechatronics.com/tutorials/arduino/how-to-control-ws2812b-individually-addressable-leds-using-arduino/
// For Pacifica effect
#define FASTLED_ALLOW_INTERRUPTS 0

#include <SoftwareSerial.h>
#include "FastLED.h"
FASTLED_USING_NAMESPACE

#define NUM_LEDS 16
#define LED_PIN 8
#define COLOR_ORDER GRB
#define FRAMES_PER_SECOND 100
//#define MAX_POWER_MILLIAMPS 400
#define LED_TYPE WS2812

CRGB leds[NUM_LEDS];

SoftwareSerial Bluetooth(0, 1);  // Arduino(RX, TX) - Bluetooth (TX, RX)

// Initial background color
// https://www.rapidtables.com/web/color/RGB_Color.html
byte backR = 128;
byte backG = 128;
byte backB = 0;

// Initial reactive color
byte reactiveR = 10;
byte reactiveG = 50;
byte reactiveB = 100;

int brightness = 10;  // Initial brightness

String currentMode = "T";   // Initial mode: table. It can be T: table, E: effect
String effectType = "R";    // R: random, C: custom
String customEffect = "1";  // Pacifica
String dataIn = "";

void setup() {
  delay(3000);  // 3 second delay for recovery

  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  //FastLED.setMaxPowerInVoltsAndMilliamps(5, MAX_POWER_MILLIAMPS);
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
SimplePatternList gPatterns = { rainbow, rainbowWithGlitter, confetti, backforthpoint, juggle, coloredstripes };
uint8_t gCurrentPatternNumber = 0;  // Index number of which pattern is current
uint8_t gHue = 0;                   // rotating "base color" used by many of the patterns

void loop() {
  if (Bluetooth.available() > 0) {
    dataIn = Bluetooth.readString();
    Serial.print("Received: [");
    Serial.print(dataIn);
    Serial.println("]");
    delay(20);

    if (dataIn.startsWith("T")) {
      // Table
      setTableConfig();
    } else if (dataIn.startsWith("E")) {
      // Effects
      setEffectConfig();
    } else if (dataIn.startsWith("I")) {
      // Intensity (brightness)
      setBrightnessConfig();
    }
  }

  // In mode E we should update the demo effects
  if (currentMode == "E") {
    delay(10);
    processEffect();
  }
}

void initialize() {
  //Serial.println(F("Initializing table..."));

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

void setTableConfig() {
  // Format: T[K,F][RxxxGxxxBxxxE]
  // T: table
  // K: background color
  // F: foreground color
  // RGB values

  currentMode = "T";
  //Serial.println(F("Mode: T"));

  if (dataIn.substring(1, 2) == "K") {
    // K: background color

    //Serial.print(F("BACK: "));


    String stringR = dataIn.substring(dataIn.indexOf("R") + 1, dataIn.indexOf("G"));
    // Serial.print(F("StringR: "));
    // Serial.println(stringR);
    backR = stringR.toInt();
    String stringG = dataIn.substring(dataIn.indexOf("G") + 1, dataIn.indexOf("B"));
    // Serial.print(F("StringG: "));
    // Serial.println(stringG);
    backG = stringG.toInt();
    String stringB = dataIn.substring(dataIn.indexOf("B") + 1, dataIn.indexOf("E"));
    // Serial.print(F("StringB: "));
    // Serial.println(stringB);
    backB = stringB.toInt();

  } else if (dataIn.substring(1, 2) == "F") {
    // F: foreground color
    String stringR = dataIn.substring(dataIn.indexOf("R") + 1, dataIn.indexOf("G"));
    reactiveR = stringR.toInt();
    String stringG = dataIn.substring(dataIn.indexOf("G") + 1, dataIn.indexOf("B"));
    reactiveG = stringG.toInt();
    String stringB = dataIn.substring(dataIn.indexOf("B") + 1, dataIn.indexOf("E"));
    reactiveB = stringB.toInt();
  }

  updateTable();
}

void updateTable() {
  delay(10);

  // Serial.println(F("Updating Table..."));
  // Serial.print(F("R: "));
  // Serial.println(backR);
  // Serial.print(F("G: "));
  // Serial.println(backG);
  // Serial.print(F("B: "));
  // Serial.println(backB);

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

void setBrightnessConfig() {
  // Format: Ixxxxx
  // I: Intensity (brigthness)
  // xxxxx: brightness value
  //Serial.println(F("Mode: I"));
  String stringBrightness = dataIn.substring(dataIn.indexOf("I") + 1, dataIn.length());
  brightness = stringBrightness.toInt();
  FastLED.setBrightness(brightness);
  FastLED.delay(1000 / FRAMES_PER_SECOND);
}

void setEffectConfig() {
  // Format: E[R,C][C:1,2,3,4,5...n]
  // E: effect
  // R: random effect
  // C: custom effect
  // For C, the custom effect selected
  // Serial.println(F("Mode: E"));
  // Serial.print(F("Data: "));
  // Serial.println(dataIn);
  currentMode = "E";
  effectType = dataIn.substring(1, 2);  // R: random, C: custom
  customEffect = dataIn.substring(2, dataIn.length());
  // Serial.print(F("EffectType: "));
  // Serial.println(effectType);
  // Serial.print(F("CustomId: "));
  // Serial.println(customEffect);
}

//****************************************************************************
// *** START Random effects
//****************************************************************************

#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))

void processEffect() {
  // Custom effects:
  // 1: pacifica
  // 2: rainbow (FastLED's built-in rainbow generator)
  // 3: rainbowWithGlitter (built-in FastLED rainbow, plus some random sparkly glitter)
  // 4: confetti (random colored speckles that blink in and fade smoothly)
  // 5: backforthpoint (a colored dot sweeping back and forth, with fading trails)
  // 6: juggle (eight colored dots, weaving in and out of sync with each other)
  // 7: coloredstripes (colored stripes pulsing at a defined Beats-Per-Minute (BPM))

  // { rainbow, rainbowWithGlitter, confetti, backforthpoint, juggle, coloredstripes };

  if (effectType == "R") {
    executeEffect(-1);
  } else if (effectType == "C") {
    if (customEffect == "1") {
      pacifica();
    } else if (customEffect == "2") {
      // rainbow (0);
      executeEffect(0);
    } else if (customEffect == "3") {
      // rainbowWithGlitter (1);
      executeEffect(1);
    } else if (customEffect == "4") {
      //confetti();
      executeEffect(2);
    } else if (customEffect == "5") {
      //backforthpoint();
      executeEffect(3);
    } else if (customEffect == "6") {
      //juggle();
      executeEffect(4);
    } else if (customEffect == "7") {
      //coloredstripes();
      executeEffect(5);
    }
  }
}

void executeEffect(int effectNumber) {
  // Call the current pattern function once, updating the 'leds' array

  if (effectNumber > -1) {
    gPatterns[effectNumber]();
  } else {
    gPatterns[gCurrentPatternNumber]();
  }

  // send the 'leds' array out to the actual LED strip
  FastLED.setBrightness(brightness);
  FastLED.show();
  // insert a delay to keep the framerate modest
  FastLED.delay(1000 / FRAMES_PER_SECOND);

  if (effectNumber > -1) {
    // do some periodic updates
    EVERY_N_MILLISECONDS(50) {
      gHue++;
    }
  } else {
    // do some periodic updates
    EVERY_N_MILLISECONDS(50) {
      gHue++;
    }  // slowly cycle the "base color" through the rainbow
    EVERY_N_SECONDS(20) {
      nextPattern();
    }
  }
  // change patterns periodically
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

void backforthpoint() {
  // a colored dot sweeping back and forth, with fading trails
  fadeToBlackBy(leds, NUM_LEDS, 20);
  int pos = beatsin16(13, 0, NUM_LEDS - 1);
  leds[pos] += CHSV(gHue, 255, 192);
}

void coloredstripes() {
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

//------------------------------
// Pacifica Effect
//------------------------------
//////////////////////////////////////////////////////////////////////////
//
// The code for this animation is more complicated than other examples, and
// while it is "ready to run", and documented in general, it is probably not
// the best starting point for learning.  Nevertheless, it does illustrate some
// useful techniques.
//
//////////////////////////////////////////////////////////////////////////
//
// In this animation, there are four "layers" of waves of light.
//
// Each layer moves independently, and each is scaled separately.
//
// All four wave layers are added together on top of each other, and then
// another filter is applied that adds "whitecaps" of brightness where the
// waves line up with each other more.  Finally, another pass is taken
// over the led array to 'deepen' (dim) the blues and greens.
//
// The speed and scale and motion each layer varies slowly within independent
// hand-chosen ranges, which is why the code has a lot of low-speed 'beatsin8' functions
// with a lot of oddly specific numeric ranges.
//
// These three custom blue-green color palettes were inspired by the colors found in
// the waters off the southern coast of California, https://goo.gl/maps/QQgd97jjHesHZVxQ7
//
CRGBPalette16 pacifica_palette_1 = { 0x000507, 0x000409, 0x00030B, 0x00030D, 0x000210, 0x000212, 0x000114, 0x000117,
                                     0x000019, 0x00001C, 0x000026, 0x000031, 0x00003B, 0x000046, 0x14554B, 0x28AA50 };
CRGBPalette16 pacifica_palette_2 = { 0x000507, 0x000409, 0x00030B, 0x00030D, 0x000210, 0x000212, 0x000114, 0x000117,
                                     0x000019, 0x00001C, 0x000026, 0x000031, 0x00003B, 0x000046, 0x0C5F52, 0x19BE5F };
CRGBPalette16 pacifica_palette_3 = { 0x000208, 0x00030E, 0x000514, 0x00061A, 0x000820, 0x000927, 0x000B2D, 0x000C33,
                                     0x000E39, 0x001040, 0x001450, 0x001860, 0x001C70, 0x002080, 0x1040BF, 0x2060FF };


void pacifica() {
  EVERY_N_MILLISECONDS(20) {
    pacifica_loop();
    FastLED.show();
    // insert a delay to keep the framerate modest
    FastLED.delay(1000 / FRAMES_PER_SECOND);
  }
}

void pacifica_loop() {
  // Increment the four "color index start" counters, one for each wave layer.
  // Each is incremented at a different speed, and the speeds vary over time.
  static uint16_t sCIStart1, sCIStart2, sCIStart3, sCIStart4;
  static uint32_t sLastms = 0;
  uint32_t ms = GET_MILLIS();
  uint32_t deltams = ms - sLastms;
  sLastms = ms;
  uint16_t speedfactor1 = beatsin16(3, 179, 269);
  uint16_t speedfactor2 = beatsin16(4, 179, 269);
  uint32_t deltams1 = (deltams * speedfactor1) / 256;
  uint32_t deltams2 = (deltams * speedfactor2) / 256;
  uint32_t deltams21 = (deltams1 + deltams2) / 2;
  sCIStart1 += (deltams1 * beatsin88(1011, 10, 13));
  sCIStart2 -= (deltams21 * beatsin88(777, 8, 11));
  sCIStart3 -= (deltams1 * beatsin88(501, 5, 7));
  sCIStart4 -= (deltams2 * beatsin88(257, 4, 6));

  // Clear out the LED array to a dim background blue-green
  fill_solid(leds, NUM_LEDS, CRGB(2, 6, 10));

  // Render each of four layers, with different scales and speeds, that vary over time
  pacifica_one_layer(pacifica_palette_1, sCIStart1, beatsin16(3, 11 * 256, 14 * 256), beatsin8(10, 70, 130), 0 - beat16(301));
  pacifica_one_layer(pacifica_palette_2, sCIStart2, beatsin16(4, 6 * 256, 9 * 256), beatsin8(17, 40, 80), beat16(401));
  pacifica_one_layer(pacifica_palette_3, sCIStart3, 6 * 256, beatsin8(9, 10, 38), 0 - beat16(503));
  pacifica_one_layer(pacifica_palette_3, sCIStart4, 5 * 256, beatsin8(8, 10, 28), beat16(601));

  // Add brighter 'whitecaps' where the waves lines up more
  pacifica_add_whitecaps();

  // Deepen the blues and greens a bit
  pacifica_deepen_colors();
}

// Add one layer of waves into the led array
void pacifica_one_layer(CRGBPalette16& p, uint16_t cistart, uint16_t wavescale, uint8_t bri, uint16_t ioff) {
  uint16_t ci = cistart;
  uint16_t waveangle = ioff;
  uint16_t wavescale_half = (wavescale / 2) + 20;
  for (uint16_t i = 0; i < NUM_LEDS; i++) {
    waveangle += 250;
    uint16_t s16 = sin16(waveangle) + 32768;
    uint16_t cs = scale16(s16, wavescale_half) + wavescale_half;
    ci += cs;
    uint16_t sindex16 = sin16(ci) + 32768;
    uint8_t sindex8 = scale16(sindex16, 240);
    CRGB c = ColorFromPalette(p, sindex8, bri, LINEARBLEND);
    leds[i] += c;
  }
}

// Add extra 'white' to areas where the four layers of light have lined up brightly
void pacifica_add_whitecaps() {
  uint8_t basethreshold = beatsin8(9, 55, 65);
  uint8_t wave = beat8(7);

  for (uint16_t i = 0; i < NUM_LEDS; i++) {
    uint8_t threshold = scale8(sin8(wave), 20) + basethreshold;
    wave += 7;
    uint8_t l = leds[i].getAverageLight();
    if (l > threshold) {
      uint8_t overage = l - threshold;
      uint8_t overage2 = qadd8(overage, overage);
      leds[i] += CRGB(overage, overage2, qadd8(overage2, overage2));
    }
  }
}

// Deepen the blues and greens
void pacifica_deepen_colors() {
  for (uint16_t i = 0; i < NUM_LEDS; i++) {
    leds[i].blue = scale8(leds[i].blue, 145);
    leds[i].green = scale8(leds[i].green, 200);
    leds[i] |= CRGB(2, 5, 7);
  }
}
//------------------------------
// End Pacifica Effect
//------------------------------


//****************************************************************************
// *** END Random effects
//****************************************************************************
