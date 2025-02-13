#include <Arduino.h>
#include <Wire.h>

#define PLATFORM_AMB82

#ifdef PLATFORM_ARDUINO
#include "sensors/imu.h"
#include "sensors/ps.h"
#include "sensors/mag.h"
#endif

#ifdef PLATFORM_AMB82
#include "sensors/imu.cpp"
#include "sensors/ps.cpp"
#include "sensors/mag.cpp"
#include "datafile/datafile.cpp"
#endif

IMU* imu;
PS* ps;
MAG* mag;

DataFile* datafile;

void setup()
{
  Serial.begin(9600);

  Wire.begin();

  //imu = new IMU();
  ps = new PS();
  //mag = new MAG();

  //imu->init();
  ps->init();
  //mag->init();

  datafile = new DataFile();
}

IMU::Reading imuReading;
PS::Reading psReading;
MAG::Reading magReading;

void loop()
{
  psReading = ps->read();
  //imuReading = imu->read();
  
  //magReading = mag.read();

  /*Serial.print("acc_x:");
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
  Serial.print(imuReading.gyr.z);*/
  Serial.print("ps_p:");
  Serial.print(psReading.pres);
  Serial.print(",ps_a:");
  Serial.print(psReading.alt);
  Serial.print(",ps_t:");
  Serial.print(psReading.temp);
  /*Serial.print(",mag_x:");
  Serial.print(magReading.x);
  Serial.print(",mag_y:");
  Serial.print(magReading.y);
  Serial.print(",mag_z:");
  Serial.print(magReading.z);*/
  Serial.println();
  delay(100);
  datafile->write(String(millis()) + "," + String(psReading.alt) + "," + String(psReading.pres) + "," + String(psReading.temp));
}