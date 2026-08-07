#include <Arduino.h>

// AlphaBot V2-Ar - Task 07: Buzzer Beep
// Buzzer pin: D10

const uint8_t BUZZER_PIN = 10;

void beep(uint16_t frequencyHz, uint16_t durationMs) {
  tone(BUZZER_PIN, frequencyHz, durationMs);
  delay(durationMs + 30);
  noTone(BUZZER_PIN);
}

void shortBeep() {
  beep(1200, 120);
}

void longBeep() {
  beep(1200, 350);
}

void playSOS() {
  const uint16_t dot = 120;
  const uint16_t dash = 360;
  const uint16_t gap = 120;

  for (uint8_t i = 0; i < 3; i++) {
    tone(BUZZER_PIN, 1200, dot);
    delay(dot + gap);
  }

  delay(180);

  for (uint8_t i = 0; i < 3; i++) {
    tone(BUZZER_PIN, 1200, dash);
    delay(dash + gap);
  }

  delay(180);

  for (uint8_t i = 0; i < 3; i++) {
    tone(BUZZER_PIN, 1200, dot);
    delay(dot + gap);
  }
}

void setup() {
  pinMode(BUZZER_PIN, OUTPUT);
  noTone(BUZZER_PIN);
  Serial.begin(9600);
  Serial.println(F("07_buzzer_beep ready"));
}

void loop() {
  shortBeep();
  delay(300);

  shortBeep();
  delay(500);

  longBeep();
  delay(700);

  playSOS();
  delay(1200);
}