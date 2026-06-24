/**
 * @file config.h
 * @brief AlphaBot V2-Ar Master Configuration Header
 * @description DEPRECATED: Use Configuration/config.h instead
 * @note This file is kept for backward compatibility only.
 *       All configuration should be done via Configuration/config.h and Configuration/pins.h
 */

#ifndef CONFIG_H
#define CONFIG_H

// Include the authoritative configuration files
#include "Configuration/pins.h"
#include "Configuration/config.h"

#endif // CONFIG_H
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
