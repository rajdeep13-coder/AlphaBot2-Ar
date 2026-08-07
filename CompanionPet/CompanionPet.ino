/**
 * @file CompanionPet.ino
 * @brief AlphaBot2 Desktop Companion Pet — main sketch.
 *
 * Hardware (all permanently connected):
 *   TB6612FNG motors      D4-D7
 *   HC-SR04 ultrasonic    D8, D9
 *   Passive buzzer        D10   (fallback audio)
 *   IR receiver           D11
 *   WS2812B x4            D12
 *   SSD1306 OLED          A4/A5 (I2C)
 *   KY-038 sound sensor   D2
 *   SG90 servo (tail)     D3
 *   TTP223 touch pad      A0
 *   DFPlayer Mini         A1 (TX), A2 (RX)
 *
 * Line sensors (A0-A2) are NOT connected — those pins are in use by pet HW.
 * D13 = built-in LED, unused.
 *
 * No delay() in loop. All animation, lighting, sound, and motor gestures
 * are non-blocking.
 *
 * Libraries (install via Arduino Library Manager):
 *   Adafruit SSD1306, Adafruit GFX, Adafruit NeoPixel, DFRobotDFPlayerMini
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

  face.begin();  // Non-fatal — warns if OLED missing, bot runs anyway
  Serial.println(face.hasDisplay()
    ? F("[INIT] OLED      OK")
    : F("[INIT] OLED      MISSING (running without display)"));

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

  Serial.println(F("[BOOT] Ready — all systems active."));
  Serial.println(F("       Touch pad (A0) to pet."));
  Serial.println(F("       Wave hand near ultrasonic to interact."));
  Serial.println(F("       Clap/sound triggers ALERT state."));
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