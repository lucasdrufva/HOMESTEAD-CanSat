#include "time_sensor.h"

#include <Arduino.h>
#include "time_data.h"

void TimeSensor::run()
{
    if(millis() - lastUpdate >= period)
    {
        TimeData *data = new TimeData(millis());
        dataCallback(data);
    }
}