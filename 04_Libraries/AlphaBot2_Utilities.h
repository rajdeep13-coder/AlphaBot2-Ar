/**
 * @file AlphaBot2_Utilities.h
 * @brief Utility Functions for AlphaBot V2-Ar
 * @description Helper functions for timing, math, buzzer, and debugging
 * @author Your Name
 * @version 1.0
 */

#ifndef ALPHABOT2_UTILITIES_H
#define ALPHABOT2_UTILITIES_H

#include <Arduino.h>
#include "../05_Configuration/pins.h"
#include "../05_Configuration/config.h"

// ============================================================================
// BUZZER CLASS
// ============================================================================

class AlphaBot2_Buzzer {
private:
  int buzzerPin;

public:
  AlphaBot2_Buzzer(int pin) {
    buzzerPin = pin;
  }

  void begin() {
    pinMode(buzzerPin, OUTPUT);
    noTone(buzzerPin);
  }

  void beep(int frequency, int duration) {
    tone(buzzerPin, frequency, duration);
  }

  void beepError() {
    beep(TONE_ERROR, TONE_SHORT_DURATION);
  }

  void beepWarning() {
    beep(TONE_WARNING, TONE_SHORT_DURATION);
  }

  void beepSuccess() {
    beep(TONE_SUCCESS, TONE_SHORT_DURATION);
  }

  void beepAlert() {
    beep(TONE_ALERT, TONE_MEDIUM_DURATION);
  }

  void playMelody(const int* frequencies, const int* durations, int count) {
    for (int i = 0; i < count; i++) {
      tone(buzzerPin, frequencies[i], durations[i]);
      delay(durations[i] + 50);
    }
    noTone(buzzerPin);
  }

  void stop() {
    noTone(buzzerPin);
  }
};

// ============================================================================
// TIMER CLASS
// ============================================================================

class AlphaBot2_Timer {
private:
  unsigned long startTime;
  unsigned long duration;
  bool isRunning;

public:
  AlphaBot2_Timer(unsigned long durationMs) {
    duration = durationMs;
    isRunning = false;
  }

  void start() {
    startTime = millis();
    isRunning = true;
  }

  void stop() {
    isRunning = false;
  }

  bool isFinished() {
    if (!isRunning) return false;
    return (millis() - startTime) >= duration;
  }

  unsigned long getElapsed() {
    if (!isRunning) return 0;
    return millis() - startTime;
  }

  int getPercentage() {
    if (!isRunning) return 0;
    unsigned long elapsed = getElapsed();
    return (elapsed * 100) / duration;
  }

  void setDuration(unsigned long durationMs) {
    duration = durationMs;
  }
};

// ============================================================================
// AVERAGING FILTER
// ============================================================================

class AlphaBot2_Filter {
private:
  int* buffer;
  int bufferSize;
  int index;
  int sum;

public:
  AlphaBot2_Filter(int size) {
    bufferSize = size;
    buffer = new int[size];
    index = 0;
    sum = 0;
    for (int i = 0; i < size; i++) {
      buffer[i] = 0;
    }
  }

  ~AlphaBot2_Filter() {
    delete[] buffer;
  }

  int add(int value) {
    sum -= buffer[index];
    buffer[index] = value;
    sum += buffer[index];
    index = (index + 1) % bufferSize;
    return getAverage();
  }

  int getAverage() {
    return sum / bufferSize;
  }

  void reset() {
    index = 0;
    sum = 0;
    for (int i = 0; i < bufferSize; i++) {
      buffer[i] = 0;
    }
  }
};

// ============================================================================
// PID CONTROLLER
// ============================================================================

class AlphaBot2_PID {
private:
  float kp, ki, kd;
  float prevError;
  float integral;

public:
  AlphaBot2_PID(float p, float i, float d) {
    kp = p;
    ki = i;
    kd = d;
    prevError = 0;
    integral = 0;
  }

  float calculate(float error) {
    integral += error;
    integral = constrain(integral, -100, 100);  // Anti-windup

    float derivative = error - prevError;
    float output = (kp * error) + (ki * integral) + (kd * derivative);
    prevError = error;

    return output;
  }

  void reset() {
    prevError = 0;
    integral = 0;
  }

  void setConstants(float p, float i, float d) {
    kp = p;
    ki = i;
    kd = d;
  }
};

// ============================================================================
// DEBUG UTILITIES
// ============================================================================

class AlphaBot2_Debug {
public:
  static void begin(unsigned long baudRate = SERIAL_BAUD_RATE) {
    Serial.begin(baudRate);
  }

  static void print(const char* text) {
    if (DEBUG_SERIAL) Serial.print(text);
  }

  static void println(const char* text) {
    if (DEBUG_SERIAL) Serial.println(text);
  }

  static void printValue(const char* label, int value) {
    if (DEBUG_SERIAL) {
      Serial.print(label);
      Serial.print(": ");
      Serial.println(value);
    }
  }

  static void printMotor(int speedA, int speedB) {
    if (DEBUG_MOTOR) {
      Serial.print("Motor A: ");
      Serial.print(speedA);
      Serial.print(" | Motor B: ");
      Serial.println(speedB);
    }
  }

  static void printSensors(int left, int center, int right) {
    if (DEBUG_SENSORS) {
      Serial.print("Sensors - L:");
      Serial.print(left);
      Serial.print(" C:");
      Serial.print(center);
      Serial.print(" R:");
      Serial.println(right);
    }
  }

  static void printStatus(const char* status) {
    if (DEBUG_SERIAL) {
      Serial.print("[");
      Serial.print(millis());
      Serial.print("] ");
      Serial.println(status);
    }
  }
};

#endif // ALPHABOT2_UTILITIES_H
