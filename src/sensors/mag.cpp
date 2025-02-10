#include "mag.h"

void MAG::init()
{
    if (!lis.init())
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