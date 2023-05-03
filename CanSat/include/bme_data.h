#pragma once

#include "data.h"



class BmeData : public Data
{
public:
    BmeData(float temperature, uint32_t pressure, float humidity, float altitude)
    {
        storedTemperature.flt = temperature;
        storedPressure = pressure;
        storedHumidity.flt = humidity;
        storedAltitude.flt = altitude;
    };
    size_t getSize()
    {
        return 4 * 4;
    }
    DataType getType()
    {
        return DATA_TYPE_BME;
    }

    void writeData(uint8_t *destination)
    {
        destination[0] = storedTemperature.u8[0];
        destination[1] = storedTemperature.u8[1];
        destination[2] = storedTemperature.u8[2];
        destination[3] = storedTemperature.u8[3];
        destination[4] = storedPressure & 0xFF;
        destination[5] = (storedPressure >> 8) & 0xFF;
        destination[6] = (storedPressure >> 16) & 0xFF;
        destination[7] = (storedPressure >> 24) & 0xFF;
        destination[8] = storedHumidity.u8[0];
        destination[9] = storedHumidity.u8[1];
        destination[10] = storedHumidity.u8[2];
        destination[11] = storedHumidity.u8[3]; 
        destination[12] = storedAltitude.u8[0];
        destination[13] = storedAltitude.u8[1];
        destination[14] = storedAltitude.u8[2];
        destination[15] = storedAltitude.u8[3]; 
    };

private:
    float_bytes storedTemperature;
    uint32_t storedPressure;
    float_bytes storedHumidity;
    float_bytes storedAltitude;
};