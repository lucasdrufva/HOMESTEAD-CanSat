#include "battery_sensor.h"

#include <Arduino.h>
#include "battery_data.h"
#include "pins.h"

bool BatterySensor::begin()
{
    pinMode(PIN_MEASSURE_BATTERY, INPUT);

    return true;
}

void BatterySensor::run()
{
    if(millis() - lastUpdate >= period)
    {
        int rawMeassurment = analogRead(PIN_MEASSURE_BATTERY);
        unsigned long converted = map(rawMeassurment, 0,1023, 0, 3300) * 1.5;

        BatteryData *data = new BatteryData(converted);
        dataCallback(data);
    }
}