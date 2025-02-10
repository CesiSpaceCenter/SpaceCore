#include <Arduino.h>
#include <Wire.h>

#include "sensors/imu.cpp"
#include "sensors/ps.cpp"
#include "sensors/mag.cpp"

IMU imu;
PS ps;
MAG mag;

void setup() {
  Serial.begin(9600);

  Wire.begin();

  imu.init();
  ps.init();
  mag.init();
}

void loop() {
}