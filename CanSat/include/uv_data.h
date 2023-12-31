#pragma once

#include "data.h"

class UvData: public Data
{
public:
    UvData(float uv, float lux)
    {
        storedUv.flt = uv;
        storedLux.flt = lux;
    };
    size_t getSize()
    {
        return 8;
    }
    DataType getType()
    {
        return DATA_TYPE_UV;
    }

    void writeData(uint8_t *destination)
    {
        destination[0] = storedUv.u8[0];
        destination[1] = storedUv.u8[1];
        destination[2] = storedUv.u8[2];
        destination[3] = storedUv.u8[3];
        destination[4] = storedLux.u8[0];
        destination[5] = storedLux.u8[1];
        destination[6] = storedLux.u8[2];
        destination[7] = storedLux.u8[3];
    };
private:
    float_bytes storedUv;
    float_bytes storedLux;
};