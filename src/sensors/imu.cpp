#include "imu.h"

void IMU::init()
{
    if (!lsm.init())
    {
        Serial.println("Failed to detect and initialize IMU!");
        while (1);
    }
    lsm.enableDefault();
}

IMU::Reading IMU::read()
{
    lsm.read();
    IMU::Reading reading;
    reading.acc = lsm.a;
    reading.gyr = lsm.g;
    return reading;
}