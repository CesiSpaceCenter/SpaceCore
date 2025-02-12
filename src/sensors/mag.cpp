#include "mag.h"

void MAG::init()
{
    if (!lis.init(LIS3MDL::device_LIS3MDL, LIS3MDL::sa1_high))
    {
        Serial.println("Failed to detect and initialize magnetometer!");
        while (1)
            ;
    }

    lis.enableDefault();
}

MAG::Reading MAG::read()
{
    lis.read();
    return lis.m;
}