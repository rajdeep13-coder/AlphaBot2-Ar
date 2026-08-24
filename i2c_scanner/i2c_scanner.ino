// --- Motor Control Pins for AlphaBot2-Ar ---
#define PWMA 6  // Left Motor Speed pin (ENA)
#define AIN2 A0 // Left Motor Forward (IN2)
#define AIN1 A1 // Left Motor Backward (IN1)

#define PWMB 5  // Right Motor Speed pin (ENB)
#define BIN1 A2 // Right Motor Forward (IN3)
#define BIN2 A3 // Right Motor Backward (IN4)

// --- Confirmed Ultrasonic Sensor Pins ---
#define TRIG_PIN 3 
#define ECHO_PIN 2 

// --- Configuration ---
#define SAFE_DISTANCE 20  // Stop if obstacle is within 20 cm
#define SPEED 130         // Motor cruising speed (0 to 255)
#define TURN_SPEED 95     // LOWERED: Slow speed for smooth, low-inertia turns
#define TURN_90_DELAY 850 // INCREASED: Since the bot turns slower, it needs more time to reach 90 degrees

void setup() {
  Serial.begin(115200);
  Serial.println("Starting Low-Inertia Smart Obstacle Avoider...");
  
  pinMode(PWMA, OUTPUT);
  pinMode(AIN1, OUTPUT);
  pinMode(AIN2, OUTPUT);
  pinMode(PWMB, OUTPUT);
  pinMode(BIN1, OUTPUT);
  pinMode(BIN2, OUTPUT);
  
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
}

void loop() {
  long distance = getDistance();
  
  if (distance > 0 && distance <= SAFE_DISTANCE) {
    Serial.println("Obstacle! Checking best path...");
    
    // Stop to kill forward momentum
    stopMotors();
    delay(300); 
    
    // 1. Look Right (90 degrees CW - Slow Turn)
    turnRight();
    delay(TURN_90_DELAY); 
    stopMotors();
    delay(400); // Wait for chassis to completely settle before pinging
    long distRight = getDistance();
    Serial.print("Right Distance: "); Serial.println(distRight);

    // 2. Look Left (180 degrees ACW to face the other way - Slow Turn)
    turnLeft();
    delay(TURN_90_DELAY * 2); 
    stopMotors();
    delay(400);
    long distLeft = getDistance();
    Serial.print("Left Distance: "); Serial.println(distLeft);

    // 3. Choose the best path
    if (distRight > distLeft) {
      Serial.println("Right is better. Turning right smoothly...");
      // We are currently facing left, so slowly turn 180 degrees CW to face right again
      turnRight();
      delay(TURN_90_DELAY * 2);
    } else {
      Serial.println("Left is better. Proceeding...");
      // We are already facing left, so we don't need to turn again!
    }
    
    // Final pause to kill turning inertia before driving straight
    stopMotors();
    delay(300);
    
  } else {
    // Path is clear, go straight!
    moveForward();
  }
  
  delay(50); 
}

// --- Sensor Function ---

long getDistance() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(5);
  
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  
  long duration = pulseIn(ECHO_PIN, HIGH, 30000); 
  
  if (duration == 0) return 999; 
  
  long dist = (duration * 0.0343) / 2;
  return dist;
}

// --- Movement Functions ---

void moveForward() {
  analogWrite(PWMA, SPEED);
  analogWrite(PWMB, SPEED);
  
  // Left Motor Forward
  digitalWrite(AIN2, HIGH);
  digitalWrite(AIN1, LOW);
  // Right Motor Forward 
  digitalWrite(BIN1, LOW);
  digitalWrite(BIN2, HIGH);
}

void turnRight() {
  analogWrite(PWMA, TURN_SPEED); 
  analogWrite(PWMB, TURN_SPEED);
  
  // Left motor forward
  digitalWrite(AIN2, HIGH);
  digitalWrite(AIN1, LOW);
  // Right motor backward
  digitalWrite(BIN1, HIGH);
  digitalWrite(BIN2, LOW);
}

void turnLeft() {
  analogWrite(PWMA, TURN_SPEED); 
  analogWrite(PWMB, TURN_SPEED);
  
  // Left motor backward
  digitalWrite(AIN2, LOW);
  digitalWrite(AIN1, HIGH);
  // Right motor forward
  digitalWrite(BIN1, LOW);
  digitalWrite(BIN2, HIGH);
}

void stopMotors() {
  analogWrite(PWMA, 0);
  analogWrite(PWMB, 0);
  
  digitalWrite(AIN2, LOW);
  digitalWrite(AIN1, LOW);
  digitalWrite(BIN1, LOW);
  digitalWrite(BIN2, LOW);
}