/**
 * @file FaceRenderer.h
 * @brief Animated eyes on the SSD1306 OLED — the pet's soul.
 *
 * All expressions are drawn procedurally with Adafruit_GFX primitives
 * (fillRoundRect, fillCircle, drawLine) — no PROGMEM bitmaps needed.
 *
 * Call setExpression() to change the face, then tick() every loop().
 * A random blink is overlaid automatically every 4-7 s.
 */

#ifndef FACE_RENDERER_H
#define FACE_RENDERER_H

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "PetConfig.h"

// ----- Eye layout constants -----
#define EYE_L_CX   32    // Left  eye centre X
#define EYE_R_CX   96    // Right eye centre X
#define EYE_CY     28    // Both  eyes centre Y

class FaceRenderer {
public:
  FaceRenderer() : _disp(SCREEN_W, SCREEN_H, &Wire, -1) {}

  bool begin() {
    if (!_disp.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR)) return false;
    _disp.clearDisplay();
    _disp.display();
    _currentExpr    = EXPR_NEUTRAL;
    _blinkFrame     = 0;
    _nextBlinkTime  = millis() + random(4000, 7001);
    _dirty          = true;
    return true;
  }

  /** Blocking wake-up animation — call once in setup(). */
  void bootAnimation() {
    // 1. Closed eyes
    _disp.clearDisplay();
    drawAsleepEyes();
    _disp.display();
    delay(500);

    // 2. Eyes opening in 4 steps (lid 80 → 60 → 40 → 20)
    for (uint8_t i = 4; i >= 1; i--) {
      _disp.clearDisplay();
      drawEyes(36, 24, 5, 0, 0, i * 20);
      _disp.display();
      delay(150);
    }

    // 3. Fully open neutral eyes
    _disp.clearDisplay();
    drawEyes(36, 24, 5, 0, 0, 0);
    _disp.display();
    delay(400);

    // 4. Brief happy flash
    _disp.clearDisplay();
    drawHappyEyes();
    _disp.display();
    delay(500);

    _currentExpr = EXPR_NEUTRAL;
    _dirty = true;
  }

  /** Set the target expression (marks dirty). */
  void setExpression(Expression expr) {
    if (expr != _currentExpr) {
      _currentExpr = expr;
      _dirty       = true;
    }
  }

  /** Call every loop() — handles blink timer and redraws when dirty. */
  void tick() {
    unsigned long now = millis();

    // --- Independent blink overlay (not during happy/asleep) ---
    if (_blinkFrame == 0) {
      if (now >= _nextBlinkTime &&
          _currentExpr != EXPR_HAPPY &&
          _currentExpr != EXPR_ASLEEP) {
        _blinkFrame = 1;
        _blinkTimer = now;
        _dirty      = true;
      }
    } else {
      if (now - _blinkTimer >= 40) {       // 40 ms per blink frame
        _blinkTimer = now;
        _blinkFrame++;
        if (_blinkFrame > 5) {             // blink complete
          _blinkFrame    = 0;
          _nextBlinkTime = now + random(4000, 7001);
        }
        _dirty = true;
      }
    }

    if (_dirty) {
      render();
      _dirty = false;
    }
  }

private:
  Adafruit_SSD1306 _disp;
  Expression       _currentExpr;
  uint8_t          _blinkFrame;      // 0 = no blink, 1-5 = blink anim
  unsigned long    _blinkTimer;
  unsigned long    _nextBlinkTime;
  bool             _dirty;

  // ================================================================
  // Master render — selects the right draw routine
  // ================================================================
  void render() {
    _disp.clearDisplay();

    // Compute eyelid percent from blink overlay
    uint8_t blinkLid = 0;
    if (_blinkFrame > 0) {
      // 1→25, 2→50, 3→100, 4→50, 5→25
      static const uint8_t bLid[] = {0, 25, 50, 100, 50, 25};
      blinkLid = bLid[_blinkFrame];
    }

    switch (_currentExpr) {
      case EXPR_NEUTRAL:
        drawEyes(36, 24, 5, 0, 0, blinkLid);
        break;

      case EXPR_WIDE:                       // Curious
        drawEyes(42, 30, 7, 0, -2, blinkLid);
        break;

      case EXPR_STARTLED:
        drawEyes(44, 34, 3, 0, 0, blinkLid);
        // Small "O" mouth
        _disp.drawCircle(64, 52, 4, SSD1306_WHITE);
        break;

      case EXPR_HAPPY:
        drawHappyEyes();
        // Small smile arc
        _disp.drawLine(54, 50, 64, 54, SSD1306_WHITE);
        _disp.drawLine(64, 54, 74, 50, SSD1306_WHITE);
        break;

      case EXPR_ALERT: {
        // Pupils dart to the side
        int8_t ox = ((millis() / 300) & 1) ? 6 : -6;  // jitter L/R
        drawEyes(36, 24, 5, ox, 0, blinkLid);
        break;
      }

      case EXPR_SLEEPY:
        drawEyes(36, 24, 5, 0, 2, max(blinkLid, (uint8_t)55));
        break;

      case EXPR_ASLEEP:
        drawAsleepEyes();
        break;
    }

    _disp.display();
  }

  // ================================================================
  // Primitive draw helpers
  // ================================================================

  /**
   * Draw a pair of cartoon eyes (white sclera, black pupil, glint).
   * @param eyeW   eye width  px
   * @param eyeH   eye height px
   * @param pupR   pupil radius px
   * @param pupOX  pupil X offset (+ = right)
   * @param pupOY  pupil Y offset (+ = down)
   * @param lidPct eyelid coverage 0-100  (0 = open, 100 = closed)
   */
  void drawEyes(uint8_t eyeW, uint8_t eyeH, uint8_t pupR,
                int8_t  pupOX, int8_t pupOY, uint8_t lidPct) {

    uint8_t r = min(eyeW, eyeH) / 3;   // corner radius

    int16_t lx = EYE_L_CX - (int16_t)eyeW / 2;
    int16_t rx = EYE_R_CX - (int16_t)eyeW / 2;
    int16_t ey = EYE_CY   - (int16_t)eyeH / 2;

    // White sclera
    _disp.fillRoundRect(lx, ey, eyeW, eyeH, r, SSD1306_WHITE);
    _disp.fillRoundRect(rx, ey, eyeW, eyeH, r, SSD1306_WHITE);

    // Black pupils
    _disp.fillCircle(EYE_L_CX + pupOX, EYE_CY + pupOY, pupR, SSD1306_BLACK);
    _disp.fillCircle(EYE_R_CX + pupOX, EYE_CY + pupOY, pupR, SSD1306_BLACK);

    // Anime-style glint (tiny white dot inside each pupil)
    if (pupR >= 4) {
      _disp.drawPixel(EYE_L_CX + pupOX + 2, EYE_CY + pupOY - 2, SSD1306_WHITE);
      _disp.drawPixel(EYE_R_CX + pupOX + 2, EYE_CY + pupOY - 2, SSD1306_WHITE);
    }

    // Eyelid overlay (black rect from top of eye)
    if (lidPct > 0) {
      uint8_t lidH = (uint16_t)eyeH * lidPct / 100;
      _disp.fillRect(lx - 1, ey - 1, eyeW + 2, lidH + 2, SSD1306_BLACK);
      _disp.fillRect(rx - 1, ey - 1, eyeW + 2, lidH + 2, SSD1306_BLACK);
    }
  }

  /** Draw happy "^ ^" eyes. */
  void drawHappyEyes() {
    for (int8_t t = -1; t <= 1; t++) {
      // Left ^
      _disp.drawLine(EYE_L_CX - 14, EYE_CY + 6,
                     EYE_L_CX,      EYE_CY - 8 + t, SSD1306_WHITE);
      _disp.drawLine(EYE_L_CX,      EYE_CY - 8 + t,
                     EYE_L_CX + 14, EYE_CY + 6,     SSD1306_WHITE);
      // Right ^
      _disp.drawLine(EYE_R_CX - 14, EYE_CY + 6,
                     EYE_R_CX,      EYE_CY - 8 + t, SSD1306_WHITE);
      _disp.drawLine(EYE_R_CX,      EYE_CY - 8 + t,
                     EYE_R_CX + 14, EYE_CY + 6,     SSD1306_WHITE);
    }
  }

  /** Draw closed-eye lines + zzZ text. */
  void drawAsleepEyes() {
    _disp.drawFastHLine(EYE_L_CX - 12, EYE_CY,     24, SSD1306_WHITE);
    _disp.drawFastHLine(EYE_L_CX - 12, EYE_CY + 1, 24, SSD1306_WHITE);
    _disp.drawFastHLine(EYE_R_CX - 12, EYE_CY,     24, SSD1306_WHITE);
    _disp.drawFastHLine(EYE_R_CX - 12, EYE_CY + 1, 24, SSD1306_WHITE);

    _disp.setTextSize(1);
    _disp.setTextColor(SSD1306_WHITE);
    _disp.setCursor(100, 4);
    _disp.print(F("zzZ"));
  }
};

#endif // FACE_RENDERER_H
