//Stuff I gotta do: PID control w/ target vel and actual vel

#define ENCODER_OPTIMIZE_INTERRUPTS

#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_MPU6050.h>
#include <Encoder.h>

/*
IMU SCL - SCL
IMU SDA - SDA

1 - Right Motor
2 - Left Motor

EN 1 - Pin 9
EN 2 - Pin 10
PH 1 - Pin 11
PH 2 - Pin 12

Hall Signal 1 A - Pin 2
Hall Signal 2 A - Pin 3
Hall Signal 1 B - Pin 4
Hall Signal 2 B - Pin 5
*/

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

float X_ACCEL_BIAS;
float Y_ACCEL_BIAS;
float Z_ACCEL_BIAS;
float X_GYRO_BIAS;
float Y_GYRO_BIAS;
float Z_GYRO_BIAS;

Adafruit_MPU6050 mpu;
Encoder encoder1(HALL1A,HALL1B);
Encoder encoder2(HALL2A,HALL2B);

void IMU_Calibration();
void print_IMU();

void setup() {
  pinMode(HALL1A, INPUT);
  pinMode(HALL2A, INPUT);
  pinMode(HALL1B, INPUT);
  pinMode(HALL2B, INPUT);

  pinMode(EN1, OUTPUT);
  pinMode(EN2, OUTPUT);
  pinMode(PH1, OUTPUT);
  pinMode(PH2, OUTPUT);

  Serial.begin(115200);
  while (!Serial){
    delay(10);
  }

  if (!mpu.begin()){
    Serial.println("Failed to find MPU6050!");
    while (1){
      delay(10);
    }
  }

  encoder1.write(0);
  encoder2.write(0);

  mpu.setAccelerometerRange(MPU6050_RANGE_16_G);
  mpu.setGyroRange(MPU6050_RANGE_250_DEG);
  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);
  Serial.println("");
  delay(100);
  IMU_Calibration();
}

void loop() {
  unsigned long current_time = millis();
  //Gets new sensor events with readings
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);

  //Give the Sensor stuff!
  
  digitalWrite(PH1, LOW);
  digitalWrite(PH2, HIGH);
  analogWrite(EN1, 50);
  analogWrite(EN2, 50);
  //Serial.println(encoder1.read());

  if ((current_time - last_time) >= interval){
    current_pos1 = encoder1.read();
    current_pos2 = encoder2.read();

    float time_delta = (current_time - last_time) / 1000.0;
    float vel_1 = (current_pos1 - last_pos1) * METERS_PER_TICK / time_delta;
    float vel_2 = (current_pos2 - last_pos2) * METERS_PER_TICK / time_delta;
    Serial.print("V:");
    Serial.print(vel_1);
    Serial.print(",");
    Serial.print(vel_2);
    Serial.print(",");

    last_pos1 = current_pos1;
    last_pos2 = current_pos2;
    last_time = current_time;

    Serial.print(a.acceleration.x - X_ACCEL_BIAS);
    Serial.print(",");
    Serial.print(a.acceleration.y - Y_ACCEL_BIAS);
    Serial.print(",");
    Serial.print(a.acceleration.z - Z_ACCEL_BIAS);
    Serial.print(",");

    Serial.print(g.gyro.x - X_GYRO_BIAS);
    Serial.print(",");
    Serial.print(g.gyro.y - Y_GYRO_BIAS);
    Serial.print(",");
    Serial.print(g.gyro.z - Z_GYRO_BIAS);
    Serial.print("\n");
  }
}

void IMU_Calibration(){

  const int CALIB_TIMESTEPS = 1000;
  double X_ACCEL_SUM = 0;
  double Y_ACCEL_SUM = 0;
  double Z_ACCEL_SUM = 0;
  double X_GYRO_SUM = 0;
  double Y_GYRO_SUM = 0;
  double Z_GYRO_SUM = 0;
  for (int i = 0; i < CALIB_TIMESTEPS; i++){
    sensors_event_t a, g, temp;
    mpu.getEvent(&a, &g, &temp);
    X_ACCEL_SUM += a.acceleration.x;
    Y_ACCEL_SUM += a.acceleration.y;
    Z_ACCEL_SUM += a.acceleration.z;

    X_GYRO_SUM += g.gyro.x;
    Y_GYRO_SUM += g.gyro.y;
    Z_GYRO_SUM += g.gyro.z;
  }

  X_ACCEL_BIAS = X_ACCEL_SUM / (float) CALIB_TIMESTEPS;
  Y_ACCEL_BIAS = Y_ACCEL_SUM / (float) CALIB_TIMESTEPS;
  Z_ACCEL_BIAS = Z_ACCEL_SUM / (float) CALIB_TIMESTEPS;
  X_GYRO_BIAS = X_GYRO_SUM / (float) CALIB_TIMESTEPS;
  Y_GYRO_BIAS = Y_GYRO_SUM / (float) CALIB_TIMESTEPS;
  Z_GYRO_BIAS = Z_GYRO_SUM / (float) CALIB_TIMESTEPS;
  Serial.print("IMU Calibrated.");

  Serial.print(X_ACCEL_BIAS);
  Serial.print(",");
  Serial.print(Y_ACCEL_BIAS);
  Serial.print(",");
  Serial.print(Z_ACCEL_BIAS);
  Serial.print(",");
  Serial.print(X_GYRO_BIAS);
  Serial.print(",");
  Serial.print(Y_GYRO_BIAS);
  Serial.print(",");
  Serial.print(Z_GYRO_BIAS);
  Serial.print("\n");
}
