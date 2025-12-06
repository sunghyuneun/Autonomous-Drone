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
    char receivedChar = Serial.read();
    if (receivedChar >= '0' && receivedChar <= '9'){
      int receivedDigit = receivedChar - '0';
      Serial.println(receivedDigit);
      analogWrite(LEDpin, analogValue(receivedDigit));
    }
  }
  // put your main code here, to run repeatedly:
}

int analogValue (int input){
  return map(input, 0, 9, 0, 255);
}