#pragma once

#include "sensor.h"

class BatterySensor: public Sensor
{
public:
    bool begin();
    UnitStatus test(){return UNIT_STATUS_OK;}
    String getName(){return "BatterySensor";}
    void run();
};