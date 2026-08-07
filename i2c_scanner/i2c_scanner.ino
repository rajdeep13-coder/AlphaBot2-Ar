/**
 * I2C Scanner — finds all connected I2C devices and their addresses.
 * Upload this sketch, open Serial Monitor at 115200 baud.
 * Look for the OLED address (typically 0x3C or 0x3D).
 * After finding it, update OLED_ADDR in PetConfig.h and re-upload CompanionPet.
 */

#include <Wire.h>

void setup() {
  Serial.begin(115200);
  Wire.begin();
  delay(500);

  Serial.println(F(""));
  Serial.println(F("=== I2C Scanner ==="));
  Serial.println(F("Scanning for devices..."));
  Serial.println(F(""));

  uint8_t found = 0;

  for (uint8_t addr = 1; addr < 127; addr++) {
    Wire.beginTransmission(addr);
    uint8_t err = Wire.endTransmission();

    if (err == 0) {
      Serial.print(F("  Device found at 0x"));
      if (addr < 16) Serial.print(F("0"));
      Serial.print(addr, HEX);

      // Known device hints
      if (addr == 0x3C || addr == 0x3D) Serial.print(F("  <-- OLED SSD1306"));
      if (addr == 0x20 || addr == 0x38) Serial.print(F("  <-- PCF8574 I/O Expander"));
      if (addr == 0x68)                 Serial.print(F("  <-- MPU6050 / DS3231 RTC"));

      Serial.println();
      found++;
    }
  }

  Serial.println(F(""));
  if (found == 0) {
    Serial.println(F("No devices found! Check wiring."));
  } else {
    Serial.print(found);
    Serial.println(F(" device(s) found."));
  }
  Serial.println(F("==================="));
}

void loop() {}
