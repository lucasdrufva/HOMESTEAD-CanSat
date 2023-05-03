#pragma once

#include <TinyGPSPlus.h>

#include "sensor.h"

class GpsSensor: public Sensor
{
public:
    bool begin();
    UnitStatus test(){return UNIT_STATUS_OK;} //TODO perform actual test
    String getName(){return "GpsSensor";}
    void run();
private:
    TinyGPSPlus gps;
    double lat = 0.0;
    double lng = 0.0;
    bool updated = false;
};