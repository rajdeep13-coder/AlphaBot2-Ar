/**
 * @file LightManager.h
 * @brief Non-blocking WS2812B mood-lighting effects for 4 NeoPixels.
 *
 * setEffect() selects a pattern; tick() advances it every ~30 ms
 * using millis()-based timing — never delay().
 */

#ifndef LIGHT_MANAGER_H
#define LIGHT_MANAGER_H

#include <Adafruit_NeoPixel.h>
#include "PetConfig.h"

class LightManager {
public:
  LightManager()
    : _strip(NUM_LEDS, RGB_LED_PIN, NEO_GRB + NEO_KHZ800) {}

  void begin() {
    _strip.begin();
    _strip.setBrightness(40);   // desk-friendly
    _strip.show();
    _effect      = LIGHT_OFF;
    _effectStart = millis();
    _lastUpdate  = 0;
  }

  void setEffect(LightEffect eff) {
    _effect      = eff;
    _effectStart = millis();
  }

  /** Call every loop(). Self-throttled to ~30 ms. */
  void tick() {
    unsigned long now = millis();
    if (now - _lastUpdate < 30) return;
    _lastUpdate = now;

    unsigned long elapsed = now - _effectStart;

    switch (_effect) {

      // ---- IDLE: slow breathing blue ----
      case LIGHT_BREATHE_BLUE: {
        uint8_t phase = (uint8_t)((elapsed % 3000UL) * 256UL / 3000UL);
        uint8_t brt   = breathe(phase);
        uint8_t b     = (uint8_t)map(brt, 0, 255, 8, 55);
        setAll(0, 0, b);
        break;
      }

      // ---- CURIOUS: cyan pulse ----
      case LIGHT_PULSE_CYAN: {
        uint8_t phase = (uint8_t)((elapsed % 1000UL) * 256UL / 1000UL);
        uint8_t brt   = breathe(phase);
        setAll(0, brt / 5, brt / 3);
        break;
      }

      // ---- STARTLED: white/red alternating flash ×4 then dim red hold ----
      case LIGHT_FLASH_WHITE_RED: {
        if (elapsed < 400) {
          uint8_t idx = (uint8_t)(elapsed / 100);
          if (idx & 1) setAll(255, 0, 0);
          else         setAll(255, 255, 255);
        } else {
          setAll(25, 0, 0);
        }
        break;
      }

      // ---- HAPPY: warm yellow-pink pulse ----
      case LIGHT_PULSE_WARM: {
        uint8_t phase = (uint8_t)((elapsed % 1500UL) * 256UL / 1500UL);
        uint8_t brt   = breathe(phase);
        uint8_t r = (uint8_t)((uint16_t)255 * brt / 255);
        uint8_t g = (uint8_t)((uint16_t)140 * brt / 255);
        uint8_t b = (uint8_t)((uint16_t) 50 * brt / 255);
        setAll(r, g, b);
        break;
      }

      // ---- ALERT: two quick white flashes ----
      case LIGHT_FLASH_WHITE_2X: {
        if      (elapsed < 120)                     setAll(255, 255, 255);
        else if (elapsed < 220)                     setAll(0, 0, 0);
        else if (elapsed < 340)                     setAll(255, 255, 255);
        else                                        setAll(0, 0, 0);
        break;
      }

      // ---- SLEEPY: slow dim-down to near-off over 5 s ----
      case LIGHT_DIM_DOWN: {
        if (elapsed < 5000UL) {
          uint8_t brt = (uint8_t)map(elapsed, 0, 5000, 40, 2);
          setAll(0, 0, brt);
        } else {
          setAll(0, 0, 2);
        }
        break;
      }

      // ---- ASLEEP: very dim slow blue pulse ----
      case LIGHT_OFF_PULSE: {
        uint8_t phase = (uint8_t)((elapsed % 8000UL) * 256UL / 8000UL);
        uint8_t brt   = breathe(phase);
        setAll(0, 0, brt / 28);      // range ~0-9
        break;
      }

      case LIGHT_OFF:
      default:
        setAll(0, 0, 0);
        break;
    }
  }

private:
  Adafruit_NeoPixel _strip;
  LightEffect       _effect;
  unsigned long     _effectStart;
  unsigned long     _lastUpdate;

  /** Smooth triangle wave with quadratic easing (0-255 → 0-252). */
  static uint8_t breathe(uint8_t phase) {
    uint16_t v = (phase < 128) ? (uint16_t)phase * 2
                               : (uint16_t)(255 - phase) * 2;
    return (uint8_t)(v * v >> 8);
  }

  void setAll(uint8_t r, uint8_t g, uint8_t b) {
    for (uint8_t i = 0; i < NUM_LEDS; i++) {
      _strip.setPixelColor(i, _strip.Color(r, g, b));
    }
    _strip.show();
  }
};

#endif // LIGHT_MANAGER_H
