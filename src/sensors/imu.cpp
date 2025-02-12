#include "imu.h"

void IMU::init()
{
    if (!lsm.init(LSM6::device_DSO, LSM6::sa0_high))
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