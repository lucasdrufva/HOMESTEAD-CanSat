#pragma once

#include "data.h"

class BatteryData: public Data
{
public:
    BatteryData(unsigned long milliVolts)
    {
        storedVoltage = milliVolts;
    };
    size_t getSize()
    {
        return 4;
    }
    DataType getType()
    {
        return DATA_TYPE_BATTERY;
    }

    void writeData(uint8_t *destination)
    {
        destination[0] = storedVoltage & 0xFF;
        destination[1] = (storedVoltage >> 8) & 0xFF;
        destination[2] = (storedVoltage >> 16) & 0xFF;
        destination[3] = (storedVoltage >> 24) & 0xFF;
    };
private:
    unsigned long storedVoltage;
};