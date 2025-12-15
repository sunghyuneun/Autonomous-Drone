#include <Arduino.h>

const int PH = 8;
const int EN = 9;

// put function declarations here:
int myFunction(int, int);

void setup() {
  // put your setup code here, to run once:
  pinMode(PH, OUTPUT);
  pinMode(EN, OUTPUT);
  digitalWrite(PH, HIGH);
}

void loop() {
  analogWrite(EN, 255);
  // put your main code here, to run repeatedly:
}

// put function definitions here:
int myFunction(int x, int y) {
  return x + y;
}