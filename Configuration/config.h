/**
 * @file config.h
 * @brief AlphaBot V2-Ar Global Configuration
 * @description Central configuration for motor speeds, sensor thresholds, and calibration
 */

#ifndef CONFIG_H
#define CONFIG_H

#include "pins.h"

// ============================================================================
// MOTOR CONFIGURATION
// ============================================================================

#define MOTOR_MIN_SPEED      0
#define MOTOR_MAX_SPEED      255
#define MOTOR_DEFAULT_SPEED  200

#define MOTOR_FORWARD   HIGH
#define MOTOR_BACKWARD  LOW

// Speed calibration factors (correct for motor differences)
#define MOTOR_A_SPEED_FACTOR 1.0
#define MOTOR_B_SPEED_FACTOR 1.0

// ============================================================================
// LINE SENSOR CALIBRATION
// ============================================================================

// Threshold values (adjust based on your track and lighting)
#define LINE_BLACK_LEFT    800
#define LINE_BLACK_CENTER  800
#define LINE_BLACK_RIGHT   800

#define LINE_WHITE_LEFT    200
#define LINE_WHITE_CENTER  200
#define LINE_WHITE_RIGHT   200

#define LINE_THRESHOLD_LEFT    ((LINE_BLACK_LEFT + LINE_WHITE_LEFT) / 2)
#define LINE_THRESHOLD_CENTER  ((LINE_BLACK_CENTER + LINE_WHITE_CENTER) / 2)
#define LINE_THRESHOLD_RIGHT   ((LINE_BLACK_RIGHT + LINE_WHITE_RIGHT) / 2)

// ============================================================================
// ULTRASONIC SENSOR CONFIGURATION
// ============================================================================

#define ULTRASONIC_MIN_DISTANCE    2    // cm
#define ULTRASONIC_MAX_DISTANCE    400  // cm
#define OBSTACLE_WARNING_DISTANCE  20   // cm

// ============================================================================
// RGB LED CONFIGURATION
// ============================================================================

#define RGB_LED_COUNT 3
#define RGB_BRIGHTNESS_MAX     255
#define RGB_BRIGHTNESS_DEFAULT 100

#define COLOR_RED      0xFF0000
#define COLOR_GREEN    0x00FF00
#define COLOR_BLUE     0x0000FF
#define COLOR_YELLOW   0xFFFF00
#define COLOR_WHITE    0xFFFFFF
#define COLOR_OFF      0x000000

// ============================================================================
// BUZZER CONFIGURATION
// ============================================================================

#define TONE_ERROR   400
#define TONE_WARNING 800
#define TONE_SUCCESS 1200
#define TONE_ALERT   2000

#define TONE_SHORT_DURATION  100
#define TONE_MEDIUM_DURATION 500
#define TONE_LONG_DURATION   1000

// ============================================================================
// JOYSTICK CONFIGURATION
// ============================================================================

#define JOYSTICK_CENTER_X   512
#define JOYSTICK_CENTER_Y   512
#define JOYSTICK_DEADZONE   50
#define JOYSTICK_MIN_VALUE  0
#define JOYSTICK_MAX_VALUE  1023

// ============================================================================
// PID CONTROL (Line Tracking)
// ============================================================================

#define PID_KP 0.8   // Proportional
#define PID_KI 0.05  // Integral
#define PID_KD 0.3   // Derivative

// ============================================================================
// COMMUNICATION
// ============================================================================

#define SERIAL_BAUD_RATE    115200
#define BLUETOOTH_BAUD_RATE 9600

// ============================================================================
// TIMING
// ============================================================================

#define MAIN_LOOP_DELAY_MS      10
#define SENSOR_READ_INTERVAL_MS 20
#define MOTOR_COMMAND_TIMEOUT_MS 5000

// ============================================================================
// DEBUG FLAGS
// ============================================================================

#define DEBUG_SERIAL        true
#define DEBUG_MOTOR         true
#define DEBUG_SENSORS       true
#define DEBUG_LINE_TRACKING true

#define DEBUG_PRINT_INTERVAL_MS 100

// ============================================================================
// FEATURE FLAGS
// ============================================================================

#define ENABLE_MOTOR_CONTROL     true
#define ENABLE_LINE_TRACKING     true
#define ENABLE_OBSTACLE_AVOID    true
#define ENABLE_RGB_LED           true
#define ENABLE_BUZZER            true
#define ENABLE_OLED              true
#define ENABLE_BLUETOOTH         false
#define ENABLE_IR_CONTROL        true

#endif // CONFIG_H
