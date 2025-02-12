#include <Arduino.h>
#include <Wire.h>

#define PLATFORM_ARDUINO

#ifdef PLATFORM_ARDUINO
#include "sensors/imu.h"
#include "sensors/ps.h"
#include "sensors/mag.h"
#endif

#ifdef PLATFORM_AMB82
#include "sensors/imu.cpp"
#include "sensors/ps.cpp"
#include "sensors/mag.cpp"
#endif

IMU imu;
PS ps;
MAG mag;

void setup()
{
  Serial.begin(9600);

  Wire.begin();

  imu.init();
  ps.init();
  mag.init();
}

char report[300];

void loop()
{
  IMU::Reading imuReading = imu.read();
  PS::Reading psReading = ps.read();
  MAG::Reading magReading = mag.read();

  Serial.print("acc_x:");
  Serial.print(imuReading.acc.x);
  Serial.print(",acc_y:");
  Serial.print(imuReading.acc.y);
  Serial.print(",acc_z:");
  Serial.print(imuReading.acc.z);
  Serial.print(",gyr_x:");
  Serial.print(imuReading.gyr.x);
  Serial.print(",gyr_y:");
  Serial.print(imuReading.gyr.y);
  Serial.print(",gyr_z:");
  Serial.print(imuReading.gyr.z);
  Serial.print(",ps_p:");
  Serial.print(psReading.pres);
  Serial.print(",ps_a:");
  Serial.print(psReading.alt);
  Serial.print(",ps_t:");
  Serial.print(psReading.temp);
  Serial.print(",mag_x:");
  Serial.print(magReading.x);
  Serial.print(",mag_y:");
  Serial.print(magReading.y);
  Serial.print(",mag_z:");
  Serial.print(magReading.z);
  Serial.println();
  delay(100);
}