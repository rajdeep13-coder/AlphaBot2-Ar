/**
 * @file SensorManager.h
 * @brief Polls all sensors, debounces, exposes clean event flags.
 *
 * Call update() every loop iteration; it self-throttles to SENSOR_POLL_MS.
 * The FSM consumes the edge-triggered flags, then they auto-clear next poll.
 */

#ifndef SENSOR_MANAGER_H
#define SENSOR_MANAGER_H

#include "PetConfig.h"

class SensorManager {
public:
  // --- Edge-triggered event flags (auto-cleared each poll cycle) ---
  bool objectNear;    // Object within PROX_NEAR_CM but not CLOSE
  bool objectClose;   // Object within PROX_CLOSE_CM (sudden!)
  bool touched;       // Touch/button just pressed (rising edge)
  bool loudSound;     // Loud noise just detected (rising edge)
  bool anyEvent;      // OR of the above four

  void begin() {
    _lastPoll       = 0;
    _touchDebounce  = 0;
    _soundDebounce  = 0;
    _distanceCm     = 999;
    _prevTouchRaw   = false;
    _prevSoundRaw   = false;

    pinMode(US_TRIG_PIN, OUTPUT);
    pinMode(US_ECHO_PIN, INPUT);

    #ifdef TOUCH_FALLBACK_PIN
      pinMode(TOUCH_FALLBACK_PIN, INPUT_PULLUP);
    #endif
    #ifdef ENABLE_TOUCH_SENSOR
      pinMode(TOUCH_PIN, INPUT);
    #endif
    #ifdef ENABLE_SOUND_SENSOR
      pinMode(SOUND_PIN, INPUT);
    #endif

    clearEvents();
  }

  /** Call every loop(). Self-throttled to SENSOR_POLL_MS. */
  void update() {
    unsigned long now = millis();
    if (now - _lastPoll < SENSOR_POLL_MS) return;
    _lastPoll = now;

    clearEvents();

    // ---- Ultrasonic distance ----
    _distanceCm = readUltrasonic();
    if (_distanceCm <= PROX_CLOSE_CM) {
      objectClose = true;
    } else if (_distanceCm <= PROX_NEAR_CM) {
      objectNear = true;
    }

    // ---- Touch detection (rising-edge) ----
    bool tRaw = readTouch();
    if (tRaw && !_prevTouchRaw && (now - _touchDebounce > 100)) {
      touched = true;
      _touchDebounce = now;
    }
    _prevTouchRaw = tRaw;

    // ---- Sound detection (rising-edge) ----
    bool sRaw = readSound();
    if (sRaw && !_prevSoundRaw && (now - _soundDebounce > 200)) {
      loudSound = true;
      _soundDebounce = now;
    }
    _prevSoundRaw = sRaw;

    anyEvent = objectNear || objectClose || touched || loudSound;
  }

  uint16_t getDistance() const { return _distanceCm; }

private:
  unsigned long _lastPoll;
  unsigned long _touchDebounce;
  unsigned long _soundDebounce;
  uint16_t      _distanceCm;
  bool          _prevTouchRaw;
  bool          _prevSoundRaw;

  void clearEvents() {
    objectNear  = false;
    objectClose = false;
    touched     = false;
    loudSound   = false;
    anyEvent    = false;
  }

  uint16_t readUltrasonic() {
    digitalWrite(US_TRIG_PIN, LOW);
    delayMicroseconds(2);
    digitalWrite(US_TRIG_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(US_TRIG_PIN, LOW);
    unsigned long dur = pulseIn(US_ECHO_PIN, HIGH, 20000UL);
    if (dur == 0) return 999;
    return (uint16_t)(dur * 0.034f * 0.5f);
  }

  bool readTouch() {
    #ifdef ENABLE_TOUCH_SENSOR
      return digitalRead(TOUCH_PIN) == HIGH;     // TTP223 active-HIGH
    #elif defined(TOUCH_FALLBACK_PIN)
      return digitalRead(TOUCH_FALLBACK_PIN) == LOW;  // Joystick btn active-LOW
    #else
      return false;
    #endif
  }

  bool readSound() {
    #ifdef ENABLE_SOUND_SENSOR
      return digitalRead(SOUND_PIN) == HIGH;     // KY-038 digital out
    #else
      return false;
    #endif
  }
};

#endif // SENSOR_MANAGER_H
