/**
 * @file AlphaBot2_Sensor.h
 * @brief Sensor Library for AlphaBot V2-Ar
 * @description Provides functions for reading line sensors, ultrasonic, and joystick
 * @author  Rajdeep Paul
 * @version 1.0
 */

#ifndef ALPHABOT2_SENSOR_H
#define ALPHABOT2_SENSOR_H

#include <Arduino.h>
#include "../Configuration/pins.h"
#include "../Configuration/config.h"

// ============================================================================
// LINE SENSOR CLASS
// ============================================================================

class AlphaBot2_LineSensor {
private:
  int pinLeft, pinCenter, pinRight;
  int threshLeft, threshCenter, threshRight;

public:
  AlphaBot2_LineSensor(int left, int center, int right) {
    pinLeft = left;
    pinCenter = center;
    pinRight = right;
    threshLeft = LINE_THRESHOLD_LEFT;
    threshCenter = LINE_THRESHOLD_CENTER;
    threshRight = LINE_THRESHOLD_RIGHT;
  }

  void begin() {
    pinMode(pinLeft, INPUT);
    pinMode(pinCenter, INPUT);
    pinMode(pinRight, INPUT);
  }

  // Read raw analog values
  int readLeft() { return analogRead(pinLeft); }
  int readCenter() { return analogRead(pinCenter); }
  int readRight() { return analogRead(pinRight); }

  // Read digital line detection (true = on line)
  bool isLineLeft() { return analogRead(pinLeft) > threshLeft; }
  bool isLineCenter() { return analogRead(pinCenter) > threshCenter; }
  bool isLineRight() { return analogRead(pinRight) > threshRight; }

  // Get line position (-2 to +2, 0 = center)
  int getLinePosition() {
    bool l = isLineLeft();
    bool c = isLineCenter();
    bool r = isLineRight();

    if (c) return 0;  // On center
    if (l) return -1; // Left of center
    if (r) return 1;  // Right of center
    return 0;
  }

  // Set custom thresholds
  void setThresholds(int left, int center, int right) {
    threshLeft = left;
    threshCenter = center;
    threshRight = right;
  }
};

// ============================================================================
// ULTRASONIC SENSOR CLASS
// ============================================================================

class AlphaBot2_Ultrasonic {
private:
  int trigPin, echoPin;

public:
  AlphaBot2_Ultrasonic(int trig, int echo) {
    trigPin = trig;
    echoPin = echo;
  }

  void begin() {
    pinMode(trigPin, OUTPUT);
    pinMode(echoPin, INPUT);
  }

  // Measure distance in centimeters
  int getDistance() {
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);

    long duration = pulseIn(echoPin, HIGH, 30000);
    int distance = duration * 0.034 / 2; // Speed of sound formula

    return constrain(distance, ULTRASONIC_MIN_DISTANCE, ULTRASONIC_MAX_DISTANCE);
  }

  // Check if obstacle is near
  bool isObstacleNear() {
    return getDistance() < OBSTACLE_WARNING_DISTANCE;
  }
};

// ============================================================================
// JOYSTICK CLASS
// ============================================================================

class AlphaBot2_Joystick {
private:
  int pinX, pinY, pinButton;
  int centerX, centerY, deadzone;

public:
  AlphaBot2_Joystick(int x, int y, int button) {
    pinX = x;
    pinY = y;
    pinButton = button;
    centerX = JOYSTICK_CENTER_X;
    centerY = JOYSTICK_CENTER_Y;
    deadzone = JOYSTICK_DEADZONE;
  }

  void begin() {
    pinMode(pinX, INPUT);
    pinMode(pinY, INPUT);
    pinMode(pinButton, INPUT_PULLUP);
  }

  // Read raw X, Y values
  int getRawX() { return analogRead(pinX); }
  int getRawY() { return analogRead(pinY); }

  // Get centered values (-512 to +512)
  int getX() {
    int raw = getRawX();
    int centered = raw - centerX;
    if (abs(centered) < deadzone) return 0;
    return centered;
  }

  int getY() {
    int raw = getRawY();
    int centered = raw - centerY;
    if (abs(centered) < deadzone) return 0;
    return centered;
  }

  // Get direction (0-7: up, up-right, right, down-right, down, down-left, left, up-left)
  int getDirection() {
    int x = getX();
    int y = getY();

    if (abs(x) < 50 && abs(y) < 50) return -1; // Center

    if (y > 50) {
      if (x > 50) return 1;      // Up-right
      if (x < -50) return 7;     // Up-left
      return 0;                  // Up
    }
    if (y < -50) {
      if (x > 50) return 3;      // Down-right
      if (x < -50) return 5;     // Down-left
      return 4;                  // Down
    }
    if (x > 50) return 2;        // Right
    if (x < -50) return 6;       // Left

    return -1;
  }

  // Check button state
  bool isPressed() {
    return digitalRead(pinButton) == LOW;
  }

  // Set deadzone
  void setDeadzone(int dz) {
    deadzone = dz;
  }
};

#endif // ALPHABOT2_SENSOR_H
