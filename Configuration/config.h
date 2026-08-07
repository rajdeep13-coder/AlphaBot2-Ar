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

#define LINE_THRESHOLD_LEFT    ((LINE_BLACK_LEFT   + LINE_WHITE_LEFT)   / 2)
#define LINE_THRESHOLD_CENTER  ((LINE_BLACK_CENTER + LINE_WHITE_CENTER) / 2)
#define LINE_THRESHOLD_RIGHT   ((LINE_BLACK_RIGHT  + LINE_WHITE_RIGHT)  / 2)

// ============================================================================
// ULTRASONIC SENSOR CONFIGURATION
// ============================================================================

#define ULTRASONIC_MIN_DISTANCE    2    // cm
#define ULTRASONIC_MAX_DISTANCE    400  // cm
#define OBSTACLE_WARNING_DISTANCE  20   // cm

// ============================================================================
// RGB LED CONFIGURATION
// ============================================================================

// RGB_LED_PIN is defined in pins.h (D12)
#define RGB_LED_COUNT          3
#define RGB_BRIGHTNESS_MAX     255
#define RGB_BRIGHTNESS_DEFAULT 100

// Predefined colors (RGB hex values)
#define COLOR_RED      0xFF0000
#define COLOR_GREEN    0x00FF00
#define COLOR_BLUE     0x0000FF
#define COLOR_YELLOW   0xFFFF00
#define COLOR_CYAN     0x00FFFF
#define COLOR_MAGENTA  0xFF00FF
#define COLOR_WHITE    0xFFFFFF
#define COLOR_OFF      0x000000

// ============================================================================
// BUZZER CONFIGURATION
// ============================================================================

// BUZZER_PIN is defined in pins.h (D10)

// Tone frequencies (Hz)
#define TONE_ERROR   400
#define TONE_WARNING 800
#define TONE_SUCCESS 1200
#define TONE_ALERT   2000

// Tone durations (milliseconds)
#define TONE_SHORT_DURATION  100
#define TONE_MEDIUM_DURATION 500
#define TONE_LONG_DURATION   1000

// ============================================================================
// JOYSTICK CONFIGURATION
// ============================================================================

#define JOYSTICK_CENTER_X  512
#define JOYSTICK_CENTER_Y  512
#define JOYSTICK_DEADZONE  50
#define JOYSTICK_MIN_VALUE 0
#define JOYSTICK_MAX_VALUE 1023

// ============================================================================
// OLED DISPLAY CONFIGURATION
// ============================================================================

#define OLED_I2C_ADDRESS 0x3C  // Default I2C address (may be 0x3D on some boards)
#define OLED_WIDTH  128        // Pixels
#define OLED_HEIGHT 64         // Pixels

// Font sizes
#define FONT_SMALL  1
#define FONT_MEDIUM 2
#define FONT_LARGE  3

// ============================================================================
// IR RECEIVER CONFIGURATION
// ============================================================================

// IR remote button codes — update these with real codes from 13_ir_receiver.ino
#define IR_BUTTON_UP     0x00
#define IR_BUTTON_DOWN   0x01
#define IR_BUTTON_LEFT   0x02
#define IR_BUTTON_RIGHT  0x03
#define IR_BUTTON_OK     0x04
#define IR_BUTTON_1      0x05
#define IR_BUTTON_2      0x06
#define IR_BUTTON_3      0x07

// ============================================================================
// DEBUG CONFIGURATION
// ============================================================================

// Enable/disable debug output
#define DEBUG_SERIAL        true   // Print debug messages to Serial
#define DEBUG_MOTOR         true   // Print motor commands
#define DEBUG_SENSORS       true   // Print sensor readings
#define DEBUG_LINE_TRACKING true   // Print line tracking data
#define DEBUG_BLUETOOTH     false  // Print Bluetooth data

// Serial print interval (to reduce spam)
#define DEBUG_PRINT_INTERVAL_MS 100  // Only print debug every 100ms

// ============================================================================
// COMMUNICATION
// ============================================================================

#define SERIAL_BAUD_RATE    115200
#define BLUETOOTH_BAUD_RATE 9600

// ============================================================================
// TIMING
// ============================================================================

#define MAIN_LOOP_DELAY_MS       10
#define SENSOR_READ_INTERVAL_MS  20
#define MOTOR_COMMAND_TIMEOUT_MS 5000

// ============================================================================
// FEATURE FLAGS
// ============================================================================

#define ENABLE_MOTOR_CONTROL      true
#define ENABLE_LINE_TRACKING      false  // A0-A2 repurposed for pet HW (touch + DFPlayer)
#define ENABLE_OBSTACLE_AVOIDANCE true
#define ENABLE_RGB_LED            true
#define ENABLE_BUZZER             true
#define ENABLE_OLED               true
#define ENABLE_IR_CONTROL         true
#define ENABLE_BLUETOOTH          false  // Disable until SoftwareSerial is configured

#endif // CONFIG_H
