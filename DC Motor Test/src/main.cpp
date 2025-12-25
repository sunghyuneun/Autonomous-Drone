#include <Arduino.h>


const int PH1 = 2;
const int EN1 = 3;
const int PH2 = 4;
const int EN2 = 5;


void setup() {
  // put your setup code here, to run once:
  pinMode(PH1, OUTPUT);
  pinMode(EN1, OUTPUT);
  pinMode(PH2, OUTPUT);
  pinMode(EN2, OUTPUT);
}

void loop() {
  digitalWrite(PH1, HIGH);
  digitalWrite(PH2, LOW);
  analogWrite(EN1, 50);
  analogWrite(EN2, 50);
  delay(1000);
  digitalWrite(PH1, LOW);
  digitalWrite(PH2, HIGH);
  analogWrite(EN1, 50);
  analogWrite(EN2, 50);
  delay(1000);
  // put your main code here, to run repeatedly:
}

// put function definitions here:
int myFunction(int x, int y) {
  return x + y;
}