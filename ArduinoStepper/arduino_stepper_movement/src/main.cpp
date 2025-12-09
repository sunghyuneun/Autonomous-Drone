#include <Arduino.h>

const int stepPin1 = 2;
const int dirPin1 = 5;
const int stepPin2 = 6;
const int dirPin2 = 7;
//67???

const int stepsPerRevo = 200;

// put function declarations here:
int myFunction(int, int);
void move(int, bool, int, bool);

void setup() {
  Serial.begin(9600);
  pinMode(stepPin1, OUTPUT);
  pinMode(dirPin1, OUTPUT);
  // put your setup code here, to run once:
  int result = myFunction(2, 3);
}

void loop() {
  if (Serial.available() > 0){
    String input = Serial.readStringUntil('\n');
    input.trim();
  }
}

void move(int step1, bool dir1, int step2, bool dir2){
  digitalWrite(dirPin1, dir1);
  for (int i = 0; i < step1, i++){
    digitalWrite(stepPin1, HIGH);
    delayMicroseconds(1000);
    digitalWrite(stepPin1, HIGH);
    delayMicroseconds(1000);
  }
}

// put function definitions here:
int myFunction(int x, int y) {
  return x + y;
}