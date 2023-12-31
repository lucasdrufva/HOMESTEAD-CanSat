#include "bme_sensor.h"

#include <Arduino.h>
#include "bme_data.h"

#define SEALEVELPRESSURE_HPA (1013.25)

bool BmeSensor::begin()
{
    if (!bme.begin(0x76))
    {
        return false;
    }

    bme.setTemperatureOversampling(BME680_OS_8X);
    bme.setHumidityOversampling(BME680_OS_2X);
    bme.setPressureOversampling(BME680_OS_4X);
    bme.setIIRFilterSize(BME680_FILTER_SIZE_3);
    bme.setGasHeater(320, 150); // 320*C for 150 ms
    endTime = bme.beginReading();
}

void BmeSensor::run()
{
    if(!(millis()-lastUpdate > period))
        return;

    lastUpdate = millis();
    
    if(millis() > (endTime+1000)){
        if(!bme.endReading()){
            return;
        }
        BmeData *data = new BmeData(bme.temperature, bme.pressure, bme.humidity, bme.readAltitude(SEALEVELPRESSURE_HPA));
        dataCallback(data);
        endTime = bme.beginReading();
    }    
}