#include <Arduino.h>

const int stepPin1 = 2;
const int dirPin1 = 5;
const int stepPin2 = 3;
const int dirPin2 = 6;

const int stepsPerRevo = 200;
const int steps = 300;
const int waitMicro = 1000;

// put function declarations here:
void move(int, int);
void accelerate(int, int, int);

void setup() {
  Serial.begin(9600);
  pinMode(stepPin1, OUTPUT);
  pinMode(dirPin1, OUTPUT);

  delay(2000);
}

void loop() {
  digitalWrite(dirPin1, HIGH);
  digitalWrite(dirPin2, LOW);
  accelerate(4000, 1000, -5);
  move(200, 1000);
  accelerate(1000, 4000, 5);
  delay(1000);
  digitalWrite(dirPin1, LOW);
  digitalWrite(dirPin2, HIGH);
  accelerate(4000, 1000, -5);
  move(200, 1000);
  accelerate(1000, 4000, 5);
  delay(1000);
}

void accelerate(int startDelay, int endDelay, int delayStep){
  int delayTime = startDelay;
  if (((endDelay - startDelay) % delayStep) != 0) {
      digitalWrite(stepPin1, HIGH);
      digitalWrite(stepPin2, HIGH);
      delayMicroseconds(300);
      digitalWrite(stepPin1, LOW);
      digitalWrite(stepPin2, LOW);
      delayMicroseconds(300);
  }
  else{
    while (delayTime != endDelay){
      digitalWrite(stepPin1, HIGH);
      digitalWrite(stepPin2, HIGH);
      delayMicroseconds(delayTime);
      digitalWrite(stepPin1, LOW);
      digitalWrite(stepPin2, LOW);
      delayMicroseconds(delayTime);
      delayTime += delayStep;
    }
  }
}

void move(int steps, int delay){
  for (int i = 0; i < steps; i++){
    digitalWrite(stepPin1, HIGH);
    digitalWrite(stepPin2, HIGH);
    delayMicroseconds(delay);
    digitalWrite(stepPin1, LOW);
    digitalWrite(stepPin2, LOW);
    delayMicroseconds(delay);
  }
}