/**
 * @file calibration.ino
 * @brief Sensor Calibration Tools for AlphaBot V2-Ar
 * @description Run this sketch to calibrate line sensors, motors, and other components
 * @usage Upload, open Serial Monitor (115200 baud), and follow on-screen instructions
 */

#include <Arduino.h>
#include "pins.h"
#include "config.h"

// ============================================================================
// CALIBRATION GLOBALS
// ============================================================================

int sensorMin[3] = {1023, 1023, 1023};
int sensorMax[3] = {0, 0, 0};

// ============================================================================
// SETUP
// ============================================================================

void setup() {
  // Initialize pins
  pinMode(MOTOR_A_PWM_PIN, OUTPUT);
  pinMode(MOTOR_A_DIR_PIN, OUTPUT);
  pinMode(MOTOR_B_PWM_PIN, OUTPUT);
  pinMode(MOTOR_B_DIR_PIN, OUTPUT);
  
  pinMode(LINE_SENSOR_LEFT, INPUT);
  pinMode(LINE_SENSOR_CENTER, INPUT);
  pinMode(LINE_SENSOR_RIGHT, INPUT);

  Serial.begin(SERIAL_BAUD_RATE);
  
  Serial.println("\n========================================");
  Serial.println("  AlphaBot V2-Ar Calibration Tool");
  Serial.println("========================================\n");
  Serial.println("Select calibration mode:");
  Serial.println("1 - Line Sensor Calibration");
  Serial.println("2 - Motor Speed Test");
  Serial.println("3 - Joystick Calibration");
  Serial.println("4 - Full Sensor Diagnostics");
  Serial.println("\nEnter choice (1-4): ");
}

// ============================================================================
// MAIN LOOP
// ============================================================================

void loop() {
  if (Serial.available()) {
    char choice = Serial.read();
    
    switch (choice) {
      case '1':
        calibrateLineSensors();
        break;
      case '2':
        testMotorSpeed();
        break;
      case '3':
        calibrateJoystick();
        break;
      case '4':
        diagnosticAll();
        break;
      default:
        Serial.println("Invalid choice. Please enter 1-4.");
        break;
    }
    
    printMenu();
  }
}

// ============================================================================
// LINE SENSOR CALIBRATION
// ============================================================================

void calibrateLineSensors() {
  Serial.println("\n--- Line Sensor Calibration ---");
  Serial.println("Place robot on WHITE surface and press any key...");
  waitForInput();
  
  // Record white values
  int whiteLeft = 0, whiteCenter = 0, whiteRight = 0;
  for (int i = 0; i < 100; i++) {
    whiteLeft += analogRead(LINE_SENSOR_LEFT);
    whiteCenter += analogRead(LINE_SENSOR_CENTER);
    whiteRight += analogRead(LINE_SENSOR_RIGHT);
    delay(10);
  }
  whiteLeft /= 100;
  whiteCenter /= 100;
  whiteRight /= 100;
  
  Serial.print("White values - L: ");
  Serial.print(whiteLeft);
  Serial.print(" C: ");
  Serial.print(whiteCenter);
  Serial.print(" R: ");
  Serial.println(whiteRight);
  
  Serial.println("\nPlace robot on BLACK line and press any key...");
  waitForInput();
  
  // Record black values
  int blackLeft = 0, blackCenter = 0, blackRight = 0;
  for (int i = 0; i < 100; i++) {
    blackLeft += analogRead(LINE_SENSOR_LEFT);
    blackCenter += analogRead(LINE_SENSOR_CENTER);
    blackRight += analogRead(LINE_SENSOR_RIGHT);
    delay(10);
  }
  blackLeft /= 100;
  blackCenter /= 100;
  blackRight /= 100;
  
  Serial.print("Black values - L: ");
  Serial.print(blackLeft);
  Serial.print(" C: ");
  Serial.print(blackCenter);
  Serial.print(" R: ");
  Serial.println(blackRight);
  
  Serial.println("\nCalibrized thresholds:");
  Serial.print("#define LINE_THRESHOLD_LEFT   ");
  Serial.println((whiteLeft + blackLeft) / 2);
  Serial.print("#define LINE_THRESHOLD_CENTER ");
  Serial.println((whiteCenter + blackCenter) / 2);
  Serial.print("#define LINE_THRESHOLD_RIGHT  ");
  Serial.println((whiteRight + blackRight) / 2);
}

// ============================================================================
// MOTOR SPEED TEST
// ============================================================================

void testMotorSpeed() {
  Serial.println("\n--- Motor Speed Test ---");
  Serial.println("Testing motor A (left) at 50%, 75%, 100% speed...");
  
  testMotor(MOTOR_A_PWM_PIN, MOTOR_A_DIR_PIN, "Motor A");
  testMotor(MOTOR_B_PWM_PIN, MOTOR_B_DIR_PIN, "Motor B");
  
  Serial.println("Motor test complete.");
}

  for (int speed = 127; speed <= 255; speed += 64) {
    Serial.print(name);
    Serial.print(" at speed ");
    Serial.print(speed);
    Serial.println(" for 2 seconds...");
    
    digitalWrite(dirPin, MOTOR_FORWARD);
    analogWrite(pwmPin, speed);
    delay(2000);
    analogWrite(pwmPin, 0);
    delay(500);
  }
}

// ============================================================================
// JOYSTICK CALIBRATION
// ============================================================================

void calibrateJoystick() {
  Serial.println("\n--- Joystick Calibration ---");
  Serial.println("Center joystick and press any key...");
  waitForInput();
  
  int centerX = 0, centerY = 0;
  for (int i = 0; i < 100; i++) {
    centerX += analogRead(JOYSTICK_X_PIN);
    centerY += analogRead(JOYSTICK_Y_PIN);
    delay(10);
  }
  centerX /= 100;
  centerY /= 100;
  
  Serial.print("Center position - X: ");
  Serial.print(centerX);
  Serial.print(" Y: ");
  Serial.println(centerY);
  
  Serial.println("\nUpdate config.h with:");
  Serial.print("#define JOYSTICK_CENTER_X  ");
  Serial.println(centerX);
  Serial.print("#define JOYSTICK_CENTER_Y  ");
  Serial.println(centerY);
}

// ============================================================================
// FULL DIAGNOSTICS
// ============================================================================

void diagnosticAll() {
  Serial.println("\n--- Full System Diagnostics ---");
  Serial.println("Sampling all analog sensors for 5 seconds...\n");
  
  unsigned long startTime = millis();
  while (millis() - startTime < 5000) {
    Serial.print("L:");
    Serial.print(analogRead(LINE_SENSOR_LEFT));
    Serial.print(" C:");
    Serial.print(analogRead(LINE_SENSOR_CENTER));
    Serial.print(" R:");
    Serial.print(analogRead(LINE_SENSOR_RIGHT));
    Serial.print(" | JX:");
    Serial.print(analogRead(JOYSTICK_X_PIN));
    Serial.print(" JY:");
    Serial.println(analogRead(JOYSTICK_Y_PIN));
    delay(200);
  }
  
  Serial.println("\nDiagnostics complete.");
}

// ============================================================================
// UTILITY FUNCTIONS
// ============================================================================

void waitForInput() {
  while (!Serial.available()) {
    delay(10);
  }
  Serial.read(); // consume the character
  Serial.println("Starting...");
}

void printMenu() {
  delay(1000);
  Serial.println("\n--- Select Next Mode ---");
  Serial.println("1 - Line Sensor Calibration");
  Serial.println("2 - Motor Speed Test");
  Serial.println("3 - Joystick Calibration");
  Serial.println("4 - Full Sensor Diagnostics");
  Serial.println("Enter choice (1-4): ");
}
