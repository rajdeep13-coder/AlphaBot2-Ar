/**
 * @file config.h
 * @brief AlphaBot V2-Ar Configuration Header
 * @description Central configuration file for all sensor pins, calibration values, and constants
 * @author Your Name
 * @version 1.0
 * @date 2026-06-20
 */

#ifndef CONFIG_H
#define CONFIG_H

// ============================================================================
// MOTOR CONFIGURATION
// ============================================================================

// Motor A (Left Motor)
#define MOTOR_A_PWM_PIN    5
#define MOTOR_A_DIR_PIN    4

// Motor B (Right Motor)
#define MOTOR_B_PWM_PIN    6
#define MOTOR_B_DIR_PIN    7

// Motor Speed Range
#define MOTOR_MIN_SPEED    0
#define MOTOR_MAX_SPEED    255
#define MOTOR_DEFAULT_SPEED 200

// Motor Direction
#define MOTOR_FORWARD      HIGH
#define MOTOR_BACKWARD     LOW

// ============================================================================
// LINE SENSOR CONFIGURATION
// ============================================================================

// Analog pins for line sensors
#define LINE_SENSOR_LEFT   A0
#define LINE_SENSOR_CENTER A1
#define LINE_SENSOR_RIGHT  A2

// Sensor calibration values (adjust based on your testing)
// BLACK LINE reading (typically higher on dark surface)
#define LINE_BLACK_LEFT    800
#define LINE_BLACK_CENTER  800
#define LINE_BLACK_RIGHT   800

// WHITE SURFACE reading (typically lower on light surface)
#define LINE_WHITE_LEFT    200
#define LINE_WHITE_CENTER  200
#define LINE_WHITE_RIGHT   200

// Threshold for binary detection
#define LINE_THRESHOLD_LEFT   ((LINE_BLACK_LEFT + LINE_WHITE_LEFT) / 2)
#define LINE_THRESHOLD_CENTER ((LINE_BLACK_CENTER + LINE_WHITE_CENTER) / 2)
#define LINE_THRESHOLD_RIGHT  ((LINE_BLACK_RIGHT + LINE_WHITE_RIGHT) / 2)

// ============================================================================
// ULTRASONIC SENSOR CONFIGURATION
// ============================================================================

#define ULTRASONIC_TRIG_PIN 8
#define ULTRASONIC_ECHO_PIN 9

// Distance limits
#define ULTRASONIC_MIN_DISTANCE 2    // cm (minimum detectable distance)
#define ULTRASONIC_MAX_DISTANCE 400  // cm (maximum detectable distance)
#define OBSTACLE_WARNING_DISTANCE 20 // cm (alert when obstacle closer than this)

// ============================================================================
// RGB LED CONFIGURATION
// ============================================================================

#define RGB_LED_PIN 12
#define RGB_LED_COUNT 3  // Number of addressable LEDs on robot

// LED brightness
#define RGB_BRIGHTNESS_MAX 255
#define RGB_BRIGHTNESS_DEFAULT 100

// Predefined colors (RGB values)
#define COLOR_RED    0xFF0000
#define COLOR_GREEN  0x00FF00
#define COLOR_BLUE   0x0000FF
#define COLOR_YELLOW 0xFFFF00
#define COLOR_CYAN   0x00FFFF
#define COLOR_MAGENTA 0xFF00FF
#define COLOR_WHITE  0xFFFFFF
#define COLOR_OFF    0x000000

// ============================================================================
// BUZZER CONFIGURATION
// ============================================================================

#define BUZZER_PIN 10

// Tone frequencies (Hz)
#define TONE_ERROR    400
#define TONE_WARNING  800
#define TONE_SUCCESS  1200
#define TONE_ALERT    2000

// Tone durations (milliseconds)
#define TONE_SHORT_DURATION 100
#define TONE_MEDIUM_DURATION 500
#define TONE_LONG_DURATION 1000

// ============================================================================
// IR RECEIVER CONFIGURATION
// ============================================================================

#define IR_RECEIVER_PIN 11

// IR remote button codes (customize based on your remote)
#define IR_BUTTON_UP     0x0
#define IR_BUTTON_DOWN   0x1
#define IR_BUTTON_LEFT   0x2
#define IR_BUTTON_RIGHT  0x3
#define IR_BUTTON_OK     0x4
#define IR_BUTTON_1      0x5
#define IR_BUTTON_2      0x6

// ============================================================================
// JOYSTICK CONFIGURATION
// ============================================================================

#define JOYSTICK_X_PIN     A6
#define JOYSTICK_Y_PIN     A7
#define JOYSTICK_BUTTON_PIN 2

// Joystick center position (typically 512 for 0-1023 range)
#define JOYSTICK_CENTER_X  512
#define JOYSTICK_CENTER_Y  512

// Dead zone (ignore inputs within this range of center)
#define JOYSTICK_DEADZONE  50

// Direction thresholds
#define JOYSTICK_MIN_VALUE 0
#define JOYSTICK_MAX_VALUE 1023

// ============================================================================
// OLED DISPLAY CONFIGURATION
// ============================================================================

#define OLED_I2C_ADDRESS 0x3C  // Default I2C address (may be 0x3D on some boards)
#define OLED_WIDTH  128        // Pixels
#define OLED_HEIGHT 64         // Pixels

// Font sizes
#define FONT_SMALL 1
#define FONT_MEDIUM 2
#define FONT_LARGE 3

// ============================================================================
// CALIBRATION VALUES
// ============================================================================

// Motor speed calibration (to correct for motor differences)
#define MOTOR_A_SPEED_FACTOR 1.0  // Multiply left motor speed by this factor
#define MOTOR_B_SPEED_FACTOR 1.0  // Multiply right motor speed by this factor

// Line tracking PID coefficients (tune these values)
#define PID_KP 0.8    // Proportional gain
#define PID_KI 0.05   // Integral gain
#define PID_KD 0.3    // Derivative gain

// ============================================================================
// COMMUNICATION CONFIGURATION
// ============================================================================

// Serial communication baud rate
#define SERIAL_BAUD_RATE 115200

// Bluetooth module (HC-05) parameters
#define BLUETOOTH_BAUD_RATE 9600
#define BLUETOOTH_RX_PIN 0  // Software serial RX
#define BLUETOOTH_TX_PIN 1  // Software serial TX

// ============================================================================
// TIMING CONFIGURATION
// ============================================================================

// Loop timing
#define MAIN_LOOP_DELAY_MS 10      // Main loop delay in milliseconds
#define SENSOR_READ_INTERVAL_MS 20 // Sensor reading interval

// Motor timing
#define MOTOR_COMMAND_TIMEOUT_MS 5000 // Stop motor if no command for 5 seconds

// ============================================================================
// DEBUGGING & LOGGING
// ============================================================================

// Enable/disable debug output
#define DEBUG_SERIAL true      // Print debug messages to Serial
#define DEBUG_MOTOR true       // Print motor commands
#define DEBUG_SENSORS true     // Print sensor readings
#define DEBUG_LINE_TRACKING true // Print line tracking data
#define DEBUG_BLUETOOTH false  // Print Bluetooth data

// Serial print intervals (to reduce spam)
#define DEBUG_PRINT_INTERVAL_MS 100 // Only print debug every 100ms

// ============================================================================
// FEATURE FLAGS
// ============================================================================

// Enable/disable features
#define ENABLE_MOTOR_CONTROL true
#define ENABLE_LINE_TRACKING true
#define ENABLE_OBSTACLE_AVOIDANCE true
#define ENABLE_RGB_LED true
#define ENABLE_BUZZER true
#define ENABLE_OLED true
#define ENABLE_BLUETOOTH false  // Disable until fully configured
#define ENABLE_IR_CONTROL false // Disable until fully configured

// ============================================================================
// BATTERY & POWER CONFIGURATION
// ============================================================================

// Battery monitoring (if implemented)
#define BATTERY_LOW_VOLTAGE 6.0   // volts
#define BATTERY_CRITICAL_VOLTAGE 5.5 // volts

// Power saving settings
#define ENABLE_POWER_SAVING false // Disable motor after inactivity
#define POWER_SAVING_TIMEOUT_MS 30000 // 30 seconds of inactivity

// ============================================================================
// PHYSICAL MEASUREMENTS
// ============================================================================

// Robot dimensions (in mm)
#define ROBOT_WIDTH 110
#define ROBOT_LENGTH 110
#define ROBOT_HEIGHT 56

// Wheel specifications
#define WHEEL_DIAMETER_MM 42
#define WHEEL_CIRCUMFERENCE_MM (WHEEL_DIAMETER_MM * 3.14159) // 131.95 mm

// Distance between wheels (track width)
#define TRACK_WIDTH_MM 100

// Motor specifications
#define MOTOR_GEAR_RATIO 30 // 1:30 reduction ratio
#define MOTOR_RPM_NO_LOAD 600 // RPM at 6V no load

// ============================================================================
// ALGORITHM PARAMETERS
// ============================================================================

// Line tracking parameters
#define LINE_MAX_ERROR 3 // Maximum sensor deviation
#define LINE_BASE_SPEED 200 // Base motor speed for line tracking

// Maze solver parameters
#define MAZE_MAX_DEPTH 20 // Maximum recursion depth for maze solving

// ============================================================================
// SAFETY LIMITS
// ============================================================================

// Motor speed limits (prevent damage)
#define MAX_MOTOR_SPEED_FORWARD 255
#define MAX_MOTOR_SPEED_BACKWARD 200  // Slower backward to prevent tip-over

// Current limiting (if implemented)
#define MOTOR_CURRENT_LIMIT_MA 500 // Motor current limit in milliamps

// Temperature limits (if temperature sensor added)
#define MAX_MOTOR_TEMPERATURE 60  // Celsius
#define BATTERY_MAX_TEMPERATURE 50 // Celsius

#endif // CONFIG_H
