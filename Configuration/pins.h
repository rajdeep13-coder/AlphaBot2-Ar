/**
 * @file pins.h
 * @brief Pin Definitions for AlphaBot V2-Ar
 * @description Central header defining all Arduino pin assignments
 */

#ifndef PINS_H
#define PINS_H

// ============================================================================
// MOTOR PINS (TB6612FNG Dual H-Bridge Driver)
// ============================================================================

#define MOTOR_A_PWM_PIN    5     // Left motor speed control (PWM)
#define MOTOR_A_DIR_PIN    4     // Left motor direction control
#define MOTOR_B_PWM_PIN    6     // Right motor speed control (PWM)
#define MOTOR_B_DIR_PIN    7     // Right motor direction control

// ============================================================================
// SENSOR PINS
// ============================================================================

// Line Sensors (A0-A2) — NOT CONNECTED in Companion Pet build
// These pins are repurposed:
//   A0 → TTP223 touch pad (TOUCH_PIN in PetConfig.h)
//   A1 → DFPlayer Mini SoftwareSerial TX (DFP_TX_PIN)
//   A2 → DFPlayer Mini SoftwareSerial RX (DFP_RX_PIN)
// #define LINE_SENSOR_LEFT   A0  // REPURPOSED
// #define LINE_SENSOR_CENTER A1  // REPURPOSED
// #define LINE_SENSOR_RIGHT  A2  // REPURPOSED

// Ultrasonic Distance Sensor (HC-SR04)
#define ULTRASONIC_TRIG_PIN 8
#define ULTRASONIC_ECHO_PIN 9

// Infrared Remote Receiver
#define IR_RECEIVER_PIN 11

// ============================================================================
// OUTPUT PINS
// ============================================================================

// RGB LED (WS2812B NeoPixel)
#define RGB_LED_PIN 12

// Buzzer
#define BUZZER_PIN 10

// ============================================================================
// INPUT PINS
// ============================================================================

// Joystick
#define JOYSTICK_X_PIN     A6
#define JOYSTICK_Y_PIN     A7
#define JOYSTICK_BUTTON_PIN 2

// ============================================================================
// COMMUNICATION PINS
// ============================================================================

// OLED Display (I2C)
#define OLED_SDA_PIN A4
#define OLED_SCL_PIN A5

// Bluetooth Module (HC-05) — DISABLED
// D0/D1 conflict with USB upload; ENABLE_BLUETOOTH = false in config.h
// Do not connect HC-05 during development.
// #define BLUETOOTH_RX_PIN 0
// #define BLUETOOTH_TX_PIN 1

// ============================================================================
// POWER PINS
// ============================================================================

// Battery power: VCC (7.4V from 2x 14500 cells)
// Ground: GND (common)
// USB Power: 5V regulated output

#endif // PINS_H
