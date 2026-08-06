/**
 * @file MotionGestures.h
 * @brief Non-blocking motor gesture sequences for personality.
 *
 * Each gesture is a tiny state-machine driven by millis().
 * Motor commands use the 4-pin TB6612FNG scheme (DIR + PWM).
 * All speeds are desk-safe (60-100 / 255).
 */

#ifndef MOTION_GESTURES_H
#define MOTION_GESTURES_H

#include "PetConfig.h"

#ifdef ENABLE_SERVO
  #include <Servo.h>
#endif

class MotionGestures {
public:
  void begin() {
    pinMode(MOTOR_A_PWM, OUTPUT);
    pinMode(MOTOR_A_DIR, OUTPUT);
    pinMode(MOTOR_B_PWM, OUTPUT);
    pinMode(MOTOR_B_DIR, OUTPUT);
    motorStop();

    _gesture   = GESTURE_NONE;
    _step      = 0;
    _active    = false;
    _swayDelay = 5000;

    #ifdef ENABLE_SERVO
      _servo.attach(SERVO_PIN);
      _servo.write(90);               // centre
    #endif
  }

  /** Start a gesture (cancels any running one). */
  void startGesture(GestureType g) {
    _gesture   = g;
    _step      = 0;
    _stepStart = millis();
    _active    = true;
    if (g == GESTURE_IDLE_SWAY) {
      _swayDelay = random(3000, 8001);
      motorStop();
    }
    applyStepEntry();
  }

  /** Cancel the current gesture and stop motors. */
  void stopGesture() {
    _active  = false;
    _gesture = GESTURE_NONE;
    motorStop();
  }

  bool isActive() const { return _active; }

  /** Advance the current gesture — call every loop(). */
  void tick() {
    if (!_active) return;
    unsigned long elapsed = millis() - _stepStart;

    switch (_gesture) {
      case GESTURE_IDLE_SWAY:   tickIdleSway(elapsed);   break;
      case GESTURE_LOOK_TOWARD: tickLookToward(elapsed);  break;
      case GESTURE_BACK_UP:     tickBackUp(elapsed);      break;
      case GESTURE_WIGGLE:      tickWiggle(elapsed);      break;
      case GESTURE_TURN_TOWARD: tickTurnToward(elapsed);  break;
      default: _active = false; break;
    }
  }

private:
  GestureType   _gesture;
  uint8_t       _step;
  unsigned long _stepStart;
  bool          _active;
  unsigned long _swayDelay;

  #ifdef ENABLE_SERVO
    Servo _servo;
  #endif

  // ---------- Step advance helpers ----------
  void advanceStep() {
    _step++;
    _stepStart = millis();
    applyStepEntry();
  }
  void finishGesture() { motorStop(); _active = false; _gesture = GESTURE_NONE; }

  /** Called once on entering a new step — sets motor state. */
  void applyStepEntry() {
    // Most motor commands are applied inside the tick*() on step entry;
    // this is a hook for gestures that want to pre-set something.
  }

  // ================================================================
  //  IDLE SWAY  (looping)
  //  0: wait random 3-8 s  →  1: left 60ms  →  2: pause 80ms
  //  →  3: right 60ms  →  loop back to 0
  // ================================================================
  void tickIdleSway(unsigned long el) {
    switch (_step) {
      case 0:  // Waiting
        if (el >= _swayDelay) {
          _step = 1; _stepStart = millis();
          motorTurnLeft(GESTURE_SPEED_LOW);
        }
        break;
      case 1:  // Turning left
        if (el >= 60) { motorStop(); _step = 2; _stepStart = millis(); }
        break;
      case 2:  // Pause
        if (el >= 80) {
          _step = 3; _stepStart = millis();
          motorTurnRight(GESTURE_SPEED_LOW);
        }
        break;
      case 3:  // Turning right
        if (el >= 60) {
          motorStop();
          _step      = 0;
          _stepStart = millis();
          _swayDelay = random(3000, 8001);
        }
        break;
    }
  }

  // ================================================================
  //  LOOK TOWARD  (one-shot)
  //  0: turn right 150ms  →  1: pause 200ms  →  2: inch fwd 200ms
  //  →  3: stop → done
  // ================================================================
  void tickLookToward(unsigned long el) {
    switch (_step) {
      case 0:
        if (_step == 0 && el < 5) motorTurnRight(GESTURE_SPEED_MED);
        if (el >= 150) { motorStop(); advanceStep(); }
        break;
      case 1:
        if (el >= 200) { motorForward(GESTURE_SPEED_LOW); advanceStep(); }
        break;
      case 2:
        if (el >= 200) finishGesture();
        break;
    }
  }

  // ================================================================
  //  BACK UP  (one-shot)
  //  0: backward 300ms  →  1: pause 100ms  →  2: turn right 200ms
  //  →  done
  // ================================================================
  void tickBackUp(unsigned long el) {
    switch (_step) {
      case 0:
        if (el < 5) motorBackward(GESTURE_SPEED_HIGH);
        if (el >= 300) { motorStop(); advanceStep(); }
        break;
      case 1:
        if (el >= 100) { motorTurnRight(GESTURE_SPEED_MED); advanceStep(); }
        break;
      case 2:
        if (el >= 200) finishGesture();
        break;
    }
  }

  // ================================================================
  //  WIGGLE  (one-shot)
  //  4 rapid L/R oscillations, 80ms each
  // ================================================================
  void tickWiggle(unsigned long el) {
    if (_step >= 4) { finishGesture(); return; }
    if (el < 5) {
      if (_step & 1) motorTurnRight(GESTURE_SPEED_MED);
      else           motorTurnLeft(GESTURE_SPEED_MED);
    }
    if (el >= 80) { motorStop(); advanceStep(); }
  }

  // ================================================================
  //  TURN TOWARD  (one-shot)
  //  0: turn left 180ms  →  done
  // ================================================================
  void tickTurnToward(unsigned long el) {
    if (el < 5) motorTurnLeft(GESTURE_SPEED_MED);
    if (el >= 180) finishGesture();
  }

  // ================================================================
  //  Motor primitives (4-pin DIR+PWM scheme)
  // ================================================================
  static void motorForward(uint8_t speed) {
    digitalWrite(MOTOR_A_DIR, HIGH);
    digitalWrite(MOTOR_B_DIR, HIGH);
    analogWrite(MOTOR_A_PWM, speed);
    analogWrite(MOTOR_B_PWM, speed);
  }
  static void motorBackward(uint8_t speed) {
    digitalWrite(MOTOR_A_DIR, LOW);
    digitalWrite(MOTOR_B_DIR, LOW);
    analogWrite(MOTOR_A_PWM, speed);
    analogWrite(MOTOR_B_PWM, speed);
  }
  static void motorTurnLeft(uint8_t speed) {
    digitalWrite(MOTOR_A_DIR, LOW);    // left motor backward
    digitalWrite(MOTOR_B_DIR, HIGH);   // right motor forward
    analogWrite(MOTOR_A_PWM, speed);
    analogWrite(MOTOR_B_PWM, speed);
  }
  static void motorTurnRight(uint8_t speed) {
    digitalWrite(MOTOR_A_DIR, HIGH);   // left motor forward
    digitalWrite(MOTOR_B_DIR, LOW);    // right motor backward
    analogWrite(MOTOR_A_PWM, speed);
    analogWrite(MOTOR_B_PWM, speed);
  }
  static void motorStop() {
    analogWrite(MOTOR_A_PWM, 0);
    analogWrite(MOTOR_B_PWM, 0);
  }
};

#endif // MOTION_GESTURES_H
