#include <FastLED.h>

#define LED_DT 3
#define COLOR_ORDER GRB
#define LED_TYPE WS2812
#define NUM_LEDS 8
 
uint8_t max_bright=255;
struct CRGB leds[NUM_LEDS];

void setup()
{
  Serial.begin(115200);
  LEDS.addLeds<LED_TYPE, LED_DT, COLOR_ORDER>(leds, NUM_LEDS);
  FastLED.setBrightness(32);
}

void loop ()
{
  uint8_t thisSpeed = 32;
  uint8_t deltaHue= 16;
  uint8_t thisHue = beat8(thisSpeed,255);
  fill_rainbow(leds, NUM_LEDS, thisHue, deltaHue);
  FastLED.show();
}