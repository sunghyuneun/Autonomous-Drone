#include <Arduino.h>

const int LEDpin = 10;
int analogValue (int);
int lastValue = 0;

void setup() {
  Serial.begin(9600);
  pinMode(LEDpin, OUTPUT);
}

void loop() {
  if (Serial.available() > 0){
    lastValue = Serial.read();
  }
  analogWrite(LEDpin, analogValue(lastValue));
  // put your main code here, to run repeatedly:
}


int analogValue (int input){
  return map(input, 0, 10, 0, 255);
}