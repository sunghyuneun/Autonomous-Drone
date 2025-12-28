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

float vel_1 = 0.0;
float vel_2 = 0.0;

float kp = 150.0;
float ki = 40.0;
float kd = 1.0;


float target_vel_1 = 0.0;
float target_vel_2 = 0.0;
float integral1 = 0.0;
float integral2 = 0.0;
float last_error_1 = 0.0;
float last_error_2 = 0.0;

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
  unsigned long current_time = millis();
  
  if (Serial.available() > 0){
    char key = Serial.read();
    if (key == 'R'){
      target_vel_1 = -1 * Serial.parseFloat();
      Serial.print("R: ");
      Serial.println(target_vel_1);
    }
    else if (key == 'L'){
      target_vel_2 = Serial.parseFloat();
      Serial.print("L: ");
      Serial.println(target_vel_2);
    }
  }
  
  if ((current_time - last_time) >= interval){
    
    current_pos1 = encoder1.read();
    current_pos2 = encoder2.read();

    float time_delta = (current_time - last_time) / 1000.0;
    
    vel_1 = (current_pos1 - last_pos1) * METERS_PER_TICK / time_delta;
    vel_2 = (current_pos2 - last_pos2) * METERS_PER_TICK / time_delta;
  
    
    Serial.print("V:");
    Serial.print(-1 * vel_1);
    Serial.print(",");
    Serial.print(vel_2);
    Serial.print("\n");


    last_pos1 = current_pos1;
    last_pos2 = current_pos2;
    last_time = current_time;

    float error1 = target_vel_1 - vel_1;
    float error2 = target_vel_2 - vel_2;
    integral1 += error1 * time_delta;
    integral1 = constrain(integral1, -100, 100);
    integral2 += error2 * time_delta;
    integral2 = constrain(integral2, -100, 100);
    float derivative1 = (error1 - last_error_1) / time_delta;
    float derivative2 = (error2 - last_error_2) / time_delta;
    float output1 = (kp * error1) + (ki * integral1) + (kd * derivative1);
    float output2 = (kp * error2) + (ki * integral2) + (kd * derivative2);

    driveMotor(output1, output2);
    last_error_1 = error1;
    last_error_2 = error2;
  }
}


void driveMotor(float power1, float power2){
  if (power1 >= 0) digitalWrite(PH1, HIGH);
  else digitalWrite(PH1, LOW);
  if (power2 >= 0) digitalWrite(PH2, HIGH);
  else digitalWrite(PH2, LOW);
  int pwmVal1 = constrain(abs(power1), 0, 255);
  int pwmVal2 = constrain(abs(power2), 0, 255);
  analogWrite(EN1, pwmVal1);
  analogWrite(EN2, pwmVal2);
}