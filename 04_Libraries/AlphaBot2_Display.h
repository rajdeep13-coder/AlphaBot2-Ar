/**
 * @file AlphaBot2_Display.h
 * @brief Display Control Library for AlphaBot V2-Ar
 * @description Provides OLED display and RGB LED control functions
 * @author Your Name
 * @version 1.0
 */

#ifndef ALPHABOT2_DISPLAY_H
#define ALPHABOT2_DISPLAY_H

#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_NeoPixel.h>
#include "../05_Configuration/pins.h"
#include "../05_Configuration/config.h"

// ============================================================================
// OLED DISPLAY CLASS
// ============================================================================

class AlphaBot2_Display {
private:
  Adafruit_SSD1306 display;

public:
  AlphaBot2_Display() : display(OLED_WIDTH, OLED_HEIGHT, &Wire, -1) {
  }

  void begin() {
    if (!display.begin(SSD1306_SWITCHCAPVCC, OLED_I2C_ADDRESS)) {
      Serial.println("OLED initialization failed!");
      while (1);
    }
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 0);
    display.println("AlphaBot V2-Ar");
    display.display();
    delay(2000);
    clear();
  }

  void clear() {
    display.clearDisplay();
  }

  void display_data() {
    display.display();
  }

  void setTextSize(uint8_t s) {
    display.setTextSize(s);
  }

  void setTextColor(uint16_t color) {
    display.setTextColor(color);
  }

  void setCursor(int16_t x, int16_t y) {
    display.setCursor(x, y);
  }

  void println(const char* text) {
    display.println(text);
  }

  void print(const char* text) {
    display.print(text);
  }

  void println(int num) {
    display.println(num);
  }

  void print(int num) {
    display.print(num);
  }

  void drawPixel(int16_t x, int16_t y, uint16_t color) {
    display.drawPixel(x, y, color);
  }

  void drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color) {
    display.drawLine(x0, y0, x1, y1, color);
  }

  void drawCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color) {
    display.drawCircle(x0, y0, r, color);
  }

  void fillCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color) {
    display.fillCircle(x0, y0, r, color);
  }

  void drawRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color) {
    display.drawRect(x, y, w, h, color);
  }

  void fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color) {
    display.fillRect(x, y, w, h, color);
  }

  void drawBitmap(int16_t x, int16_t y, const uint8_t* bitmap, int16_t w, int16_t h, uint16_t color) {
    display.drawBitmap(x, y, bitmap, w, h, color);
  }

  void showStatus(const char* status) {
    clear();
    setTextSize(2);
    setCursor(0, 0);
    println(status);
    display_data();
  }

  void showValue(const char* label, int value) {
    clear();
    setTextSize(1);
    setCursor(0, 0);
    print(label);
    print(": ");
    println(value);
    display_data();
  }
};

// ============================================================================
// RGB LED CLASS
// ============================================================================

class AlphaBot2_RGB {
private:
  Adafruit_NeoPixel pixels;
  int numLEDs;

public:
  AlphaBot2_RGB(int pin, int count) : pixels(count, pin, NEO_GRB + NEO_KHZ800) {
    numLEDs = count;
  }

  void begin() {
    pixels.begin();
    pixels.show();
  }

  void setColor(int index, uint8_t r, uint8_t g, uint8_t b) {
    pixels.setPixelColor(index, pixels.Color(r, g, b));
  }

  void setColorAll(uint8_t r, uint8_t g, uint8_t b) {
    for (int i = 0; i < numLEDs; i++) {
      setColor(i, r, g, b);
    }
    pixels.show();
  }

  void setColorHex(int index, uint32_t color) {
    pixels.setPixelColor(index, color);
  }

  void setColorAllHex(uint32_t color) {
    for (int i = 0; i < numLEDs; i++) {
      setColorHex(i, color);
    }
    pixels.show();
  }

  void setBrightness(uint8_t brightness) {
    pixels.setBrightness(brightness);
  }

  void show() {
    pixels.show();
  }

  void clear() {
    pixels.clear();
    pixels.show();
  }

  void rainbow() {
    for (int i = 0; i < numLEDs; i++) {
      uint32_t color = pixels.ColorHSV((i * 65536L / numLEDs), 255, 255);
      setColorHex(i, color);
    }
    pixels.show();
  }

  void flash(uint32_t color, int times, int delayMs) {
    for (int i = 0; i < times; i++) {
      setColorAllHex(color);
      delay(delayMs);
      clear();
      delay(delayMs);
    }
  }
};

#endif // ALPHABOT2_DISPLAY_H
