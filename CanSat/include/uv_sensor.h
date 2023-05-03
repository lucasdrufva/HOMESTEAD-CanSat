#pragma once

#include "sensor.h"

#include "xSL01.h"

class UvSensor: public Sensor
{
public:
    bool begin();
    UnitStatus test(){return UNIT_STATUS_OK;} //TODO perform actual test
    String getName(){return "UvSensor";}
    void run();
private:
    xSL01 SL01;
};