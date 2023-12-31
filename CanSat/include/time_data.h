#pragma once

#include "data.h"

class TimeData: public Data
{
public:
    TimeData(unsigned long time)
    {
        storedTime = time;
    };
    size_t getSize()
    {
        return 4;
    }
    DataType getType()
    {
        return DATA_TYPE_TIME;
    }

    void writeData(uint8_t *destination)
    {
        destination[0] = storedTime & 0xFF;
        destination[1] = (storedTime >> 8) & 0xFF;
        destination[2] = (storedTime >> 16) & 0xFF;
        destination[3] = (storedTime >> 24) & 0xFF;
    };
private:
    unsigned long storedTime;
};