#include <Arduino.h>
#include <Adafruit_NeoPixel.h>

// AlphaBot V2-Ar RGB LED task
// Data line: D12
// Adjust LED_COUNT if your board has a different number of WS2812B LEDs.

const uint8_t RGB_PIN = 12;
const uint8_t LED_COUNT = 3;
const uint8_t BRIGHTNESS = 50;

Adafruit_NeoPixel strip(LED_COUNT, RGB_PIN, NEO_GRB + NEO_KHZ800);

const unsigned long STEP_DELAY_MS = 200;
const unsigned long PAUSE_DELAY_MS = 350;

struct ColorStep {
  uint8_t r;
  uint8_t g;
  uint8_t b;
};

const ColorStep colors[] = {
  {255, 0, 0},
  {0, 255, 0},
  {0, 0, 255},
  {255, 120, 0},
  {180, 0, 255}
};

const uint8_t COLOR_COUNT = sizeof(colors) / sizeof(colors[0]);

void setAll(uint8_t r, uint8_t g, uint8_t b) {
  for (uint8_t i = 0; i < LED_COUNT; i++) {
    strip.setPixelColor(i, strip.Color(r, g, b));
  }
  strip.show();
}

void clearAll() {
  setAll(0, 0, 0);
}

void setup() {
  Serial.begin(9600);

  strip.begin();
  strip.setBrightness(BRIGHTNESS);
  clearAll();

  Serial.println(F("04_rgb_sequential ready"));
}

void loop() {
  // Light each LED one-by-one with a changing color.
  for (uint8_t colorIndex = 0; colorIndex < COLOR_COUNT; colorIndex++) {
    for (uint8_t led = 0; led < LED_COUNT; led++) {
      clearAll();
      strip.setPixelColor(led, strip.Color(colors[colorIndex].r,
                                           colors[colorIndex].g,
                                           colors[colorIndex].b));
      strip.show();
      delay(STEP_DELAY_MS);
    }
    clearAll();
    delay(PAUSE_DELAY_MS);
  }

  // Reverse sequence for a visible loop effect.
  for (int colorIndex = COLOR_COUNT - 1; colorIndex >= 0; colorIndex--) {
    for (int led = LED_COUNT - 1; led >= 0; led--) {
      clearAll();
      strip.setPixelColor((uint8_t)led, strip.Color(colors[colorIndex].r,
                                                    colors[colorIndex].g,
                                                    colors[colorIndex].b));
      strip.show();
      delay(STEP_DELAY_MS);
    }
    clearAll();
    delay(PAUSE_DELAY_MS);
  }
}