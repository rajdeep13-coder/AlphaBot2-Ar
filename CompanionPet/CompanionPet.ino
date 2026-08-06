/**
 * @file CompanionPet.ino
 * @brief AlphaBot2 Desktop Companion Pet — main sketch.
 *
 * Hardware required (stock AlphaBot2-Ar):
 *   - TB6612FNG motors  (D4-D7)
 *   - HC-SR04 ultrasonic (D8-D9)
 *   - Buzzer             (D10)
 *   - WS2812B ×4         (D12)
 *   - SSD1306 OLED       (I2C A4/A5)
 *   - Joystick button    (D2) — acts as "pet me" until TTP223 is wired
 *
 * Optional (uncomment flags in PetConfig.h when wired):
 *   - DFPlayer Mini      (A1/A2 SoftwareSerial)
 *   - TTP223 touch pad   (A0)
 *   - KY-038 sound sensor(D2 — replaces joystick button)
 *   - SG90 servo         (D3)
 *
 * No delay() in loop. All animation, lighting, sound, and motor gestures
 * are non-blocking.
 *
 * Libraries (install via Arduino Library Manager):
 *   - Adafruit SSD1306
 *   - Adafruit GFX Library
 *   - Adafruit NeoPixel
 *   (Optional: DFRobotDFPlayerMini — only when ENABLE_DFPLAYER is set)
 */

#include <Wire.h>
#include "PetConfig.h"
#include "SensorManager.h"
#include "FaceRenderer.h"
#include "LightManager.h"
#include "SoundManager.h"
#include "MotionGestures.h"
#include "BehaviorFSM.h"

// ---- Global module instances ----
SensorManager  sensors;
FaceRenderer   face;
LightManager   lights;
SoundManager   sound;
MotionGestures motion;
BehaviorFSM    behavior;

// =================================================================
void setup() {
  Serial.begin(115200);
  Serial.println(F(""));
  Serial.println(F("================================="));
  Serial.println(F(" AlphaBot2  Companion Pet  v1.0"));
  Serial.println(F("================================="));

  // --- Seed the random generator with analog noise ---
  randomSeed(analogRead(A3));    // A3 is unconnected → floating noise

  // --- Initialise all subsystems ---
  Wire.begin();

  sensors.begin();
  Serial.println(F("[INIT] Sensors   OK"));

  if (!face.begin()) {
    Serial.println(F("[INIT] OLED FAIL — halting"));
    while (1);   // Can't run without the eyes
  }
  Serial.println(F("[INIT] OLED      OK"));

  lights.begin();
  Serial.println(F("[INIT] NeoPixel  OK"));

  sound.begin();
  Serial.println(F("[INIT] Sound     OK"));

  motion.begin();
  Serial.println(F("[INIT] Motors    OK"));

  behavior.begin(sensors, face, lights, sound, motion);
  Serial.println(F("[INIT] FSM       OK"));

  // --- Boot animation (blocking — only runs once) ---
  Serial.println(F("[BOOT] Waking up..."));
  face.bootAnimation();
  sound.playCue(SND_CHIRP);

  // --- Enter IDLE state ---
  behavior.forceState(STATE_IDLE);

  Serial.println(F("[BOOT] Ready! Press joystick button to pet."));
  Serial.println(F("       Wave hand near ultrasonic to interact."));

  #ifndef ENABLE_TOUCH_SENSOR
    Serial.println(F("       (TTP223 not enabled — using joy btn on D2)"));
  #endif
  #ifndef ENABLE_SOUND_SENSOR
    Serial.println(F("       (KY-038 not enabled — ALERT disabled)"));
  #endif
  #ifndef ENABLE_DFPLAYER
    Serial.println(F("       (DFPlayer not enabled — buzzer fallback)"));
  #endif
}

// =================================================================
void loop() {
  sensors.update();            // Self-throttled to 50 ms
  behavior.update();           // FSM transitions + periodic behaviours
  face.tick();                 // Blink timer + redraw if dirty
  lights.tick();               // Non-blocking LED effect (30 ms throttle)
  sound.tick();                // Advance buzzer melody notes
  motion.tick();               // Advance motor gesture steps
}
