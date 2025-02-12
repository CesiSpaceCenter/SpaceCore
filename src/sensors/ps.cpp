#include "ps.h"

void PS::init()
{
    if (!lps.init(LPS::device_22DF, LPS::sa0_high))
    {
        Serial.println("Failed to autodetect pressure sensor!");
        while (1)
            ;
    }

    lps.enableDefault();
}

PS::Reading PS::read()
{
    Reading reading;
    reading.pres = lps.readPressureMillibars();
    reading.alt = lps.pressureToAltitudeMeters(reading.pres);
    reading.temp = lps.readTemperatureC();
    return reading;
}