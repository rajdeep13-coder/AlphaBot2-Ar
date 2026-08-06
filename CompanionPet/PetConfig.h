/**
 * @file PetConfig.h
 * @brief Central configuration for AlphaBot2 Companion Pet
 *
 * Pin mapping follows the AlphaBot2-Ar manual (4-pin motor scheme).
 * Uncomment ENABLE_* flags as you wire each new component.
 */

#ifndef PET_CONFIG_H
#define PET_CONFIG_H

#include <Arduino.h>

// ============================================================================
// FEATURE FLAGS — Uncomment when the hardware is physically connected
// ============================================================================
// #define ENABLE_DFPLAYER       // DFPlayer Mini + 8-ohm speaker (SoftSerial on A1/A2)
// #define ENABLE_TOUCH_SENSOR   // TTP223 capacitive touch pad on A0
// #define ENABLE_SOUND_SENSOR   // KY-038 sound sensor on D2  (replaces joystick btn)
// #define ENABLE_SERVO          // SG90 micro servo for tail wag on D3

// ============================================================================
// PIN DEFINITIONS — Matches AlphaBot2-Ar manual / pins.h (Scheme A, 4-pin)
// ============================================================================

// Motor Driver (TB6612FNG)
#define MOTOR_A_PWM    5    // Left motor speed  (PWM)
#define MOTOR_A_DIR    4    // Left motor dir     (HIGH = fwd)
#define MOTOR_B_PWM    6    // Right motor speed  (PWM)
#define MOTOR_B_DIR    7    // Right motor dir    (HIGH = fwd)

// Ultrasonic Sensor (HC-SR04)
#define US_TRIG_PIN    8
#define US_ECHO_PIN    9

// Buzzer (always available as fallback audio)
#define BUZZER_PIN     10

// IR Receiver
#define IR_RECV_PIN    11

// WS2812B RGB LEDs
#define RGB_LED_PIN    12
#define NUM_LEDS       4

// OLED Display (I2C on A4/A5)
#define OLED_ADDR      0x3C
#define SCREEN_W       128
#define SCREEN_H       64

// ---------- Substitute touch when no dedicated sensor is wired ----------
// Joystick button on D2 acts as "pet me" input (active-LOW, internal pull-up)
#ifndef ENABLE_SOUND_SENSOR
  #define TOUCH_FALLBACK_PIN  2
#endif

// ---------- New-hardware pins (guarded by feature flags) ----------
#ifdef ENABLE_TOUCH_SENSOR
  #define TOUCH_PIN    A0   // TTP223 digital out (active-HIGH typical)
#endif

#ifdef ENABLE_DFPLAYER
  #define DFP_TX_PIN   A1   // Arduino TX → DFPlayer RX
  #define DFP_RX_PIN   A2   // Arduino RX ← DFPlayer TX
#endif

#ifdef ENABLE_SOUND_SENSOR
  #define SOUND_PIN    2    // KY-038 digital out
#endif

#ifdef ENABLE_SERVO
  #define SERVO_PIN    3    // SG90 PWM
#endif

// ============================================================================
// BEHAVIOUR TIMING (ms)
// ============================================================================
#define SENSOR_POLL_MS        50UL
#define SLEEPY_TIMEOUT_MS     180000UL  // 3 min idle → SLEEPY
#define ASLEEP_TIMEOUT_MS     30000UL   // 30 s sleepy → ASLEEP
#define CURIOUS_DURATION_MS   5000UL
#define STARTLED_DURATION_MS  2000UL
#define HAPPY_DURATION_MS     4000UL
#define ALERT_DURATION_MS     3000UL
#define IDLE_CHIRP_MIN_MS     20000UL   // Occasional chirp range
#define IDLE_CHIRP_MAX_MS     60000UL
#define SNORE_MIN_MS          15000UL
#define SNORE_MAX_MS          30000UL

// ============================================================================
// PROXIMITY THRESHOLDS (cm)
// ============================================================================
#define PROX_NEAR_CM   30
#define PROX_CLOSE_CM  10

// ============================================================================
// MOTOR GESTURE SPEEDS (desk-safe — low!)
// ============================================================================
#define GESTURE_SPEED_LOW   60
#define GESTURE_SPEED_MED   80
#define GESTURE_SPEED_HIGH  100

// ============================================================================
// ENUMS
// ============================================================================

enum PetState : uint8_t {
  STATE_IDLE = 0,
  STATE_CURIOUS,
  STATE_STARTLED,
  STATE_HAPPY,
  STATE_ALERT,
  STATE_SLEEPY,
  STATE_ASLEEP
};

enum Expression : uint8_t {
  EXPR_NEUTRAL = 0,
  EXPR_WIDE,       // Curious — bigger eyes
  EXPR_STARTLED,   // Max size, tiny pupils
  EXPR_HAPPY,      // ^  ^ shape
  EXPR_ALERT,      // Pupils shifted sideways
  EXPR_SLEEPY,     // Half-closed lids
  EXPR_ASLEEP      // Closed lines + zzZ
};

enum LightEffect : uint8_t {
  LIGHT_BREATHE_BLUE = 0,
  LIGHT_PULSE_CYAN,
  LIGHT_FLASH_WHITE_RED,
  LIGHT_PULSE_WARM,
  LIGHT_FLASH_WHITE_2X,
  LIGHT_DIM_DOWN,
  LIGHT_OFF_PULSE,
  LIGHT_OFF
};

enum GestureType : uint8_t {
  GESTURE_NONE = 0,
  GESTURE_IDLE_SWAY,
  GESTURE_LOOK_TOWARD,
  GESTURE_BACK_UP,
  GESTURE_WIGGLE,
  GESTURE_TURN_TOWARD
};

enum SoundCue : uint8_t {
  SND_NONE = 0,
  SND_CHIRP,     // 1 — soft idle chirp
  SND_CURIOUS,   // 2 — rising "hm?"
  SND_YELP,      // 3 — startled gasp
  SND_PURR,      // 4 — happy purr
  SND_BARK,      // 5 — alert bark
  SND_YAWN,      // 6 — sleepy yawn
  SND_SNORE      // 7 — rare snore blip
};

#endif // PET_CONFIG_H
