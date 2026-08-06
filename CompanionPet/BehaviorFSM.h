/**
 * @file BehaviorFSM.h
 * @brief Finite-state-machine brain of the companion pet.
 *
 * Owns the current PetState, evaluates sensor events with a strict
 * priority order (STARTLED > ALERT > HAPPY > CURIOUS > IDLE > SLEEPY >
 * ASLEEP), and dispatches to all output managers on state entry.
 *
 * Also handles periodic in-state behaviors:
 *   - Occasional idle chirps (20-60 s random)
 *   - Rare asleep snores   (15-30 s random)
 */

#ifndef BEHAVIOR_FSM_H
#define BEHAVIOR_FSM_H

#include "PetConfig.h"
#include "SensorManager.h"
#include "FaceRenderer.h"
#include "LightManager.h"
#include "SoundManager.h"
#include "MotionGestures.h"

class BehaviorFSM {
public:
  void begin(SensorManager& s, FaceRenderer& f, LightManager& l,
             SoundManager& snd, MotionGestures& m) {
    _sen = &s;  _face = &f;  _lgt = &l;  _snd = &snd;  _mot = &m;

    _state          = STATE_IDLE;  // will be overridden by forceState()
    _stateEntry     = millis();
    _lastEventTime  = millis();
    _nextChirpTime  = millis() + random(IDLE_CHIRP_MIN_MS, IDLE_CHIRP_MAX_MS + 1);
    _nextSnoreTime  = millis() + random(SNORE_MIN_MS, SNORE_MAX_MS + 1);
  }

  /** Force a state (used once after boot animation). */
  void forceState(PetState s) { enterState(s); }

  PetState getState() const { return _state; }

  /**
   * Main update — call every loop() AFTER sensors.update().
   * Evaluates transitions in priority order, handles timeouts,
   * drives periodic chirps/snores.
   */
  void update() {
    unsigned long now = millis();

    // Reset idle timer on ANY sensor activity
    if (_sen->anyEvent) {
      _lastEventTime = now;
    }

    // ---- 1. STARTLED (highest priority — interrupts everything) ----
    if (_sen->objectClose && _state != STATE_STARTLED) {
      enterState(STATE_STARTLED);
      return;
    }

    // ---- 2. ALERT (sound) ----
    if (_sen->loudSound &&
        _state != STATE_STARTLED && _state != STATE_ALERT) {
      enterState(STATE_ALERT);
      return;
    }

    // ---- 3. HAPPY (touch) ----
    if (_sen->touched &&
        _state != STATE_STARTLED && _state != STATE_ALERT &&
        _state != STATE_HAPPY) {
      enterState(STATE_HAPPY);
      return;
    }

    // ---- 4. Wake from SLEEPY / ASLEEP on any event ----
    if ((_state == STATE_SLEEPY || _state == STATE_ASLEEP) &&
        _sen->anyEvent) {
      enterState(STATE_IDLE);
      return;
    }

    // ---- 5. State-specific timeouts & periodic behaviours ----
    unsigned long inState = now - _stateEntry;

    switch (_state) {

      case STATE_IDLE:
        // Transition to CURIOUS if something is near
        if (_sen->objectNear) {
          enterState(STATE_CURIOUS);
          return;
        }
        // Transition to SLEEPY after long inactivity
        if (now - _lastEventTime >= SLEEPY_TIMEOUT_MS) {
          enterState(STATE_SLEEPY);
          return;
        }
        // Occasional chirp
        if (now >= _nextChirpTime) {
          _snd->playCue(SND_CHIRP);
          _nextChirpTime = now + random(IDLE_CHIRP_MIN_MS, IDLE_CHIRP_MAX_MS + 1);
        }
        break;

      case STATE_CURIOUS:
        if (inState >= CURIOUS_DURATION_MS)  enterState(STATE_IDLE);
        break;

      case STATE_STARTLED:
        if (inState >= STARTLED_DURATION_MS) enterState(STATE_IDLE);
        break;

      case STATE_HAPPY:
        if (inState >= HAPPY_DURATION_MS)    enterState(STATE_IDLE);
        break;

      case STATE_ALERT:
        if (inState >= ALERT_DURATION_MS)    enterState(STATE_IDLE);
        break;

      case STATE_SLEEPY:
        if (inState >= ASLEEP_TIMEOUT_MS)    enterState(STATE_ASLEEP);
        break;

      case STATE_ASLEEP:
        // Occasional snore
        if (now >= _nextSnoreTime) {
          _snd->playCue(SND_SNORE);
          _nextSnoreTime = now + random(SNORE_MIN_MS, SNORE_MAX_MS + 1);
        }
        break;
    }
  }

private:
  SensorManager*  _sen;
  FaceRenderer*   _face;
  LightManager*   _lgt;
  SoundManager*   _snd;
  MotionGestures* _mot;

  PetState      _state;
  unsigned long _stateEntry;
  unsigned long _lastEventTime;
  unsigned long _nextChirpTime;
  unsigned long _nextSnoreTime;

  // ------------------------------------------------------------------
  // enterState — the single dispatcher: sets face, lights, sound, motion
  // ------------------------------------------------------------------
  void enterState(PetState ns) {
    _state      = ns;
    _stateEntry = millis();

    // Any active state resets the idle timer
    if (ns != STATE_SLEEPY && ns != STATE_ASLEEP) {
      _lastEventTime = millis();
    }

    switch (ns) {

      case STATE_IDLE:
        _face->setExpression(EXPR_NEUTRAL);
        _lgt->setEffect(LIGHT_BREATHE_BLUE);
        _snd->playCue(SND_CHIRP);
        _mot->startGesture(GESTURE_IDLE_SWAY);
        _nextChirpTime = millis() + random(IDLE_CHIRP_MIN_MS, IDLE_CHIRP_MAX_MS + 1);
        break;

      case STATE_CURIOUS:
        _face->setExpression(EXPR_WIDE);
        _lgt->setEffect(LIGHT_PULSE_CYAN);
        _snd->playCue(SND_CURIOUS);
        _mot->startGesture(GESTURE_LOOK_TOWARD);
        break;

      case STATE_STARTLED:
        _face->setExpression(EXPR_STARTLED);
        _lgt->setEffect(LIGHT_FLASH_WHITE_RED);
        _snd->playCue(SND_YELP);
        _mot->startGesture(GESTURE_BACK_UP);
        break;

      case STATE_HAPPY:
        _face->setExpression(EXPR_HAPPY);
        _lgt->setEffect(LIGHT_PULSE_WARM);
        _snd->playCue(SND_PURR);
        _mot->startGesture(GESTURE_WIGGLE);
        break;

      case STATE_ALERT:
        _face->setExpression(EXPR_ALERT);
        _lgt->setEffect(LIGHT_FLASH_WHITE_2X);
        _snd->playCue(SND_BARK);
        _mot->startGesture(GESTURE_TURN_TOWARD);
        break;

      case STATE_SLEEPY:
        _face->setExpression(EXPR_SLEEPY);
        _lgt->setEffect(LIGHT_DIM_DOWN);
        _snd->playCue(SND_YAWN);
        _mot->stopGesture();
        break;

      case STATE_ASLEEP:
        _face->setExpression(EXPR_ASLEEP);
        _lgt->setEffect(LIGHT_OFF_PULSE);
        _mot->stopGesture();
        _nextSnoreTime = millis() + random(SNORE_MIN_MS, SNORE_MAX_MS + 1);
        break;
    }

    // Debug trace (F-strings → PROGMEM)
    Serial.print(F("[PET] → "));
    printStateName(ns);
    Serial.println();
  }

  static void printStateName(PetState s) {
    switch (s) {
      case STATE_IDLE:     Serial.print(F("IDLE"));     break;
      case STATE_CURIOUS:  Serial.print(F("CURIOUS"));  break;
      case STATE_STARTLED: Serial.print(F("STARTLED")); break;
      case STATE_HAPPY:    Serial.print(F("HAPPY"));    break;
      case STATE_ALERT:    Serial.print(F("ALERT"));    break;
      case STATE_SLEEPY:   Serial.print(F("SLEEPY"));   break;
      case STATE_ASLEEP:   Serial.print(F("ASLEEP"));   break;
    }
  }
};

#endif // BEHAVIOR_FSM_H
