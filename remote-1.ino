#include <Arduino.h>
#include <IRremote.h>

/*
IR Remote Keys

| Key | Action |
| --- | --- |
| 2   | Forward |
| 4   | Left |
| 6   | Right |
| 8   | Reverse |
| +   | Increase Speed |
| -   | Decrease Speed |
| 5   | Brake |
| EQ  | Reset Speed |
*/

// Motor driver pins
int PWMA = 6;
int AIN1 = A1;
int AIN2 = A0;
int PWMB = 5;
int BIN1 = A2;
int BIN2 = A3;

// IR receiver pin
const int IR_RECV_PIN = 4;
IRrecv irrecv(IR_RECV_PIN);
decode_results results;

const unsigned long KEY_2  = 0xFF18E7;   // Forward
const unsigned long KEY_8  = 0xFF4AB5;   // Reverse
const unsigned long KEY_4  = 0xFF00FF;   // Left
const unsigned long KEY_6  = 0xFF5AA5;   // Right
const unsigned long KEY_PLUS  = 0xFF629D; // Increase speed (+10)
const unsigned long KEY_MINUS = 0xFFA857; // Decrease speed (-10)
const unsigned long KEY_5   = 0xFF38C7;   // Brake
const unsigned long KEY_EQ   = 0xFF02FD;   // Reset speed

// Speed settings
int currentSpeed = 125;          // Default speed 
const int DEFAULT_SPEED = 125;
const int MAX_SPEED = 255;
const int MIN_SPEED = 0;        

// Direction states
enum Direction { NONE, FORWARD, BACKWARD, LEFT, RIGHT };
Direction currentDirection = NONE;

void setup() {
  pinMode(PWMA, OUTPUT);
  pinMode(AIN1, OUTPUT);
  pinMode(AIN2, OUTPUT);
  pinMode(PWMB, OUTPUT);
  pinMode(BIN1, OUTPUT);
  pinMode(BIN2, OUTPUT);

  stopMotors();
  currentDirection = NONE;

  Serial.begin(9600);
  irrecv.enableIRIn();
  Serial.println("IR Remote Ready. Use: 2(F), 8(B), 4(L), 6(R), +(speed+), -(speed-), 5(Brake), EQ(reset speed)");
}

void loop() {
  // 1. Check for IR commands
  if (irrecv.decode(&results)) {
    unsigned long value = results.value;

    if (value != REPEAT) {
      Serial.print("Code: 0x");
      Serial.println(value, HEX);
      handleCommand(value);
    }
    irrecv.resume();  // Ready for next code
  }

  applyMotion();
}

// ------------------------------------------------------------------
// Handle a single IR command
// ------------------------------------------------------------------
void handleCommand(unsigned long code) {
  switch (code) {
    case KEY_2:   // Forward
      currentDirection = FORWARD;
      break;
    case KEY_8:   // Backward
      currentDirection = BACKWARD;
      break;
    case KEY_4:   // Left
      currentDirection = LEFT;
      break;
    case KEY_6:   // Right
      currentDirection = RIGHT;
      break;

    case KEY_PLUS:
      currentSpeed = min(currentSpeed + 10, MAX_SPEED);
      Serial.print("Speed increased to: ");
      Serial.println(currentSpeed);
      break;

    case KEY_MINUS:
      currentSpeed = max(currentSpeed - 10, MIN_SPEED);
      Serial.print("Speed decreased to: ");
      Serial.println(currentSpeed);
      break;

    case KEY_5:   // Brake – stop immediately
      currentDirection = NONE;
      stopMotors();
      Serial.println("Brake (stop)");
      break;

    case KEY_EQ:  // Reset speed to default
      currentSpeed = DEFAULT_SPEED;
      Serial.print("Speed reset to: ");
      Serial.println(currentSpeed);
      break;

    default:
      // Unknown key – do nothing
      break;
  }
}

// ------------------------------------------------------------------
// Apply the current motion (called every loop)
// ------------------------------------------------------------------
void applyMotion() {
  if (currentDirection == NONE) {
    stopMotors();
    return;
  }

  // Set the direction pins according to currentDirection
  switch (currentDirection) {
    case FORWARD:
      digitalWrite(AIN1, LOW);
      digitalWrite(AIN2, HIGH);
      digitalWrite(BIN1, LOW);
      digitalWrite(BIN2, HIGH);
      break;
    case BACKWARD:
      digitalWrite(AIN1, HIGH);
      digitalWrite(AIN2, LOW);
      digitalWrite(BIN1, HIGH);
      digitalWrite(BIN2, LOW);
      break;
    case LEFT:   // pivot left: left motor reverse, right motor forward
      digitalWrite(AIN1, HIGH);   // Motor A (left) reverse
      digitalWrite(AIN2, LOW);
      digitalWrite(BIN1, LOW);    // Motor B (right) forward
      digitalWrite(BIN2, HIGH);
      break;
    case RIGHT:  // pivot right: left motor forward, right motor reverse
      digitalWrite(AIN1, LOW);    // Motor A forward
      digitalWrite(AIN2, HIGH);
      digitalWrite(BIN1, HIGH);   // Motor B reverse
      digitalWrite(BIN2, LOW);
      break;
    default:
      return; // shouldn't happen
  }

  // Apply the current speed to both motors
  analogWrite(PWMA, currentSpeed);
  analogWrite(PWMB, currentSpeed);
}

// ------------------------------------------------------------------
// Stop motors (set PWM to 0)
// ------------------------------------------------------------------
void stopMotors() {
  analogWrite(PWMA, 0);
  analogWrite(PWMB, 0);
}
