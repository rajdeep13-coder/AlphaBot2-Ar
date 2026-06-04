#include <Arduino.h>
#include <IRremote.h>

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

// Motor speed (0-255)
const int MOTOR_SPEED = 125;

// Movement duration (milliseconds)
const unsigned long MOVE_DURATION = 5000;  // 5 seconds

// IR codes – adjust these to match your remote
// Common NEC codes for number buttons:
const unsigned long KEY_2 = 0xFF18E7;
const unsigned long KEY_8 = 0xFF4AB5;
const unsigned long KEY_4 = 0xFF00FF;
const unsigned long KEY_6 = 0xFF5AA5;

void setup() {
  pinMode(PWMA, OUTPUT);
  pinMode(AIN1, OUTPUT);
  pinMode(AIN2, OUTPUT);
  pinMode(PWMB, OUTPUT);
  pinMode(BIN1, OUTPUT);
  pinMode(BIN2, OUTPUT);

  // Stop motors initially
  stopMotors();

  Serial.begin(9600);
  irrecv.enableIRIn();  // Start IR receiver
  Serial.println("IR remote control ready. Press 2, 8, 4, or 6.");
}

void loop() {
  if (irrecv.decode(&results)) {
    unsigned long value = results.value;

    // Ignore repeated codes (optional)
    if (value != REPEAT) {
      Serial.print("Received code: 0x");
      Serial.println(value, HEX);

      // Execute action based on received code
      switch (value) {
        case KEY_2:
          forward();
          break;
        case KEY_8:
          backward();
          break;
        case KEY_4:
          turnLeft();
          break;
        case KEY_6:
          turnRight();
          break;
        default:
          // Unknown button – do nothing
          break;
      }
    }
    irrecv.resume();  // Ready for next code
  }
}

// Move forward for 5 seconds
void forward() {
  Serial.println("Forward 5 sec");
  digitalWrite(AIN1, LOW);
  digitalWrite(AIN2, HIGH);
  digitalWrite(BIN1, LOW);
  digitalWrite(BIN2, HIGH);
  analogWrite(PWMA, MOTOR_SPEED);
  analogWrite(PWMB, MOTOR_SPEED);
  delay(MOVE_DURATION);
  stopMotors();
}

// Move backward for 5 seconds
void backward() {
  Serial.println("Backward 5 sec");
  digitalWrite(AIN1, HIGH);
  digitalWrite(AIN2, LOW);
  digitalWrite(BIN1, HIGH);
  digitalWrite(BIN2, LOW);
  analogWrite(PWMA, MOTOR_SPEED);
  analogWrite(PWMB, MOTOR_SPEED);
  delay(MOVE_DURATION);
  stopMotors();
}

// Turn left (rotate on the spot) for 5 seconds
void turnLeft() {
  Serial.println("Turn left 5 sec");
  // Left motor backward, right motor forward
  digitalWrite(AIN1, HIGH);   // Motor A reverse
  digitalWrite(AIN2, LOW);
  digitalWrite(BIN1, LOW);    // Motor B forward
  digitalWrite(BIN2, HIGH);
  analogWrite(PWMA, MOTOR_SPEED);
  analogWrite(PWMB, MOTOR_SPEED);
  delay(MOVE_DURATION);
  stopMotors();
}

// Turn right (rotate on the spot) for 5 seconds
void turnRight() {
  Serial.println("Turn right 5 sec");
  // Left motor forward, right motor backward
  digitalWrite(AIN1, LOW);    // Motor A forward
  digitalWrite(AIN2, HIGH);
  digitalWrite(BIN1, HIGH);   // Motor B reverse
  digitalWrite(BIN2, LOW);
  analogWrite(PWMA, MOTOR_SPEED);
  analogWrite(PWMB, MOTOR_SPEED);
  delay(MOVE_DURATION);
  stopMotors();
}

// Stop both motors
void stopMotors() {
  analogWrite(PWMA, 0);
  analogWrite(PWMB, 0);
}



//Code: 0xFF18E7   // button 2
//Code: 0xFF4AB5   // button 8
//Code: 0xFF00FF   // button 4
//Code: 0xFF5AA5   // button 6
