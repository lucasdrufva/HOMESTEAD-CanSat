#pragma once

#include "sensor.h"
#include "Adafruit_BME680.h"

class BmeSensor: public Sensor
{
public:
    bool begin();
    UnitStatus test(){return UNIT_STATUS_OK;} //TODO perform actual test
    String getName(){return "BmeSensor";}
    void run();
private:
    Adafruit_BME680 bme;
    unsigned long endTime = 0;
};