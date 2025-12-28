#define ENCODER_OPTIMIZE_INTERRUPTS

#include <Wire.h>
#include <Arduino.h>
#include <Encoder.h>

const int HALL1A = 2;
const int HALL2A = 3;
const int HALL1B = 4;
const int HALL2B = 5;

const int EN1 = 9;
const int EN2 = 10;
const int PH1 = 11;
const int PH2 = 12;

const float PPR = 11.0;
const float GEAR_RATIO = 157.0;
const float WHEEL_RADIUS = 0.04;
const float TICKS_PER_REV = PPR * GEAR_RATIO;
const float METERS_PER_TICK = (WHEEL_RADIUS * 2 * PI) / TICKS_PER_REV;

unsigned int last_time = 0;
long last_pos1 = 0;
long last_pos2 = 0;
long current_pos1 = 0;
long current_pos2 = 0;

const int interval = 50;


Encoder encoder1(HALL1A,HALL1B);
Encoder encoder2(HALL2A,HALL2B);

void setup() {
  // put your setup code here, to run once:
  pinMode(PH1, OUTPUT);
  pinMode(EN1, OUTPUT);
  pinMode(PH2, OUTPUT);
  pinMode(EN2, OUTPUT);

  Serial.begin(115200);
  while (!Serial){
    delay(10);
  }

  encoder1.write(0);
  encoder2.write(0);
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