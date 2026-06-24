/**
 * @file AlphaBot2_Motor.h
 * @brief Motor Control Library for AlphaBot V2-Ar
 * @description Provides functions for motor speed, direction, and movement control
 * @author Rajdeep Paul
 * @version 1.0
 */

#ifndef ALPHABOT2_MOTOR_H
#define ALPHABOT2_MOTOR_H

#include <Arduino.h>
#include "../Configuration/pins.h"
#include "../Configuration/config.h"

// ============================================================================
// MOTOR CONTROL CLASS
// ============================================================================

class AlphaBot2_Motor {
private:
  int motorA_PWM, motorA_DIR;
  int motorB_PWM, motorB_DIR;
  int speedA, speedB;

public:
  // Constructor
  AlphaBot2_Motor(int pwmA, int dirA, int pwmB, int dirB) {
    motorA_PWM = pwmA;
    motorA_DIR = dirA;
    motorB_PWM = pwmB;
    motorB_DIR = dirB;
    speedA = 0;
    speedB = 0;
  }

  // Initialize motor pins
  void begin() {
    pinMode(motorA_PWM, OUTPUT);
    pinMode(motorA_DIR, OUTPUT);
    pinMode(motorB_PWM, OUTPUT);
    pinMode(motorB_DIR, OUTPUT);
    stop();
  }

  // Set motor speeds (0-255)
  void setSpeeds(int spA, int spB) {
    speedA = constrain(spA, MOTOR_MIN_SPEED, MOTOR_MAX_SPEED);
    speedB = constrain(spB, MOTOR_MIN_SPEED, MOTOR_MAX_SPEED);
    updateMotors();
  }

  // Set speed for both motors
  void setSpeed(int speed) {
    setSpeeds(speed, speed);
  }

  // Forward motion
  void forward(int speed = MOTOR_DEFAULT_SPEED) {
    digitalWrite(motorA_DIR, MOTOR_FORWARD);
    digitalWrite(motorB_DIR, MOTOR_FORWARD);
    setSpeeds(speed, speed);
  }

  // Backward motion
  void backward(int speed = MOTOR_DEFAULT_SPEED) {
    digitalWrite(motorA_DIR, MOTOR_BACKWARD);
    digitalWrite(motorB_DIR, MOTOR_BACKWARD);
    setSpeeds(speed, speed);
  }

  // Turn left (left motor slower or reverse, right motor forward)
  void turnLeft(int speed = MOTOR_DEFAULT_SPEED) {
    digitalWrite(motorA_DIR, MOTOR_BACKWARD);
    digitalWrite(motorB_DIR, MOTOR_FORWARD);
    setSpeeds(speed, speed);
  }

  // Turn right (left motor forward, right motor slower or reverse)
  void turnRight(int speed = MOTOR_DEFAULT_SPEED) {
    digitalWrite(motorA_DIR, MOTOR_FORWARD);
    digitalWrite(motorB_DIR, MOTOR_BACKWARD);
    setSpeeds(speed, speed);
  }

  // Pivot on left motor (spin clockwise)
  void spinRight(int speed = MOTOR_DEFAULT_SPEED) {
    digitalWrite(motorA_DIR, MOTOR_FORWARD);
    digitalWrite(motorB_DIR, MOTOR_BACKWARD);
    setSpeeds(speed, speed);
  }

  // Pivot on right motor (spin counter-clockwise)
  void spinLeft(int speed = MOTOR_DEFAULT_SPEED) {
    digitalWrite(motorA_DIR, MOTOR_BACKWARD);
    digitalWrite(motorB_DIR, MOTOR_FORWARD);
    setSpeeds(speed, speed);
  }

  // Stop all motors
  void stop() {
    analogWrite(motorA_PWM, 0);
    analogWrite(motorB_PWM, 0);
    speedA = 0;
    speedB = 0;
  }

  // Get current speed of motor A
  int getSpeedA() {
    return speedA;
  }

  // Get current speed of motor B
  int getSpeedB() {
    return speedB;
  }

private:
  // Update motor PWM values
  void updateMotors() {
    analogWrite(motorA_PWM, speedA);
    analogWrite(motorB_PWM, speedB);
  }
};

#endif // ALPHABOT2_MOTOR_H
