#pragma once

#include "sensor.h"

class TimeSensor: public Sensor
{
public:
    bool begin() {return true;}
    UnitStatus test(){return UNIT_STATUS_OK;}
    String getName(){return "TimeSensor";}
    void run();
};