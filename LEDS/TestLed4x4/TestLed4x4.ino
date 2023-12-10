#include <FastLED.h>

#define NUM_LEDS 16
#define DATA_PIN 5
#define LED_TYPE WS2812
#define COLOR_ORDER GRB
#define BRIGHTNESS 10
#define FRAMES_PER_SECOND  24

CRGB leds[NUM_LEDS];

void setup() {
  delay(3000);                                                       // 3 second delay for recovery
                                                                     // tell FastLED about the LED strip configuration
  FastLED.addLeds<LED_TYPE, DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS);  //.setCorrection(TypicalLEDStrip);

  // set master brightness control
  FastLED.setBrightness(BRIGHTNESS);
}

void loop() {
  // put your main code here, to run repeatedly:
  for (int i = 0; i < 16; i++) {
    leds[i] = CRGB::MediumPurple;
    FastLED.show();
    delay(300);
  }
  FastLED.delay(1000/FRAMES_PER_SECOND); 

  for (int i = 0; i < 16; i++) {
    leds[i] = CRGB::Black;
    
    //delay(10);
  }
  FastLED.show();

}
