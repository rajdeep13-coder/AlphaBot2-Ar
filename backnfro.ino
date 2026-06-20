#include <Arduino.h>

//motor driver pins
int PWMA= 6;
int AIN1= A1;
int AIN2= A0;
int PWMB= 5;
int BIN1= A2;
int BIN2= A3;

void setup() {
  pinMode(PWMA, OUTPUT);
  pinMode(AIN1, OUTPUT);
  pinMode(AIN2, OUTPUT);
  pinMode(PWMB, OUTPUT);
  pinMode(BIN1, OUTPUT);
  pinMode(BIN2, OUTPUT);
}

void loop(){
  //forward
  digitalWrite(PWMA, HIGH);
  digitalWrite(PWMB, HIGH);
  digitalWrite(AIN1, LOW);
  digitalWrite(AIN2, HIGH);
  digitalWrite(BIN1, LOW);
  digitalWrite(BIN2, HIGH);
  delay(2000);

  //backward
  digitalWrite(AIN1, HIGH);
  digitalWrite(AIN2, LOW);
  digitalWrite(BIN1, HIGH);
  digitalWrite(BIN2, LOW);
  delay(2000);
}
