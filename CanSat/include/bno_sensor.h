#pragma once

#include "sensor.h"

#include <Adafruit_BNO08x.h>

#define BNO08X_RESET -1

class BnoSensor: public Sensor
{
public:
    BnoSensor() : bno08x(BNO08X_RESET) {};
    bool begin();
    UnitStatus test(){return UNIT_STATUS_OK;} //TODO perform actual test
    String getName(){return "BnoSensor";}
    void run();
private:
    Adafruit_BNO08x bno08x;
    sh2_SensorValue_t sensorValue;
    void setReports();
};