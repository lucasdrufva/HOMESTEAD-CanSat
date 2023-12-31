#pragma once

#include "data.h"

class GpsData : public Data
{
public:
    GpsData(double lat, double lng)
    {
        storedLat.db = lat;
        storedLng.db = lng;
    }
    size_t getSize()
    {
        return 8*2;
    }
    DataType getType()
    {
        return DATA_TYPE_GPS;
    }

    void writeData(uint8_t *destination)
    {
        destination[0] = storedLat.u8[0];
        destination[1] = storedLat.u8[1];
        destination[2] = storedLat.u8[2];
        destination[3] = storedLat.u8[3];
        destination[4] = storedLat.u8[4];
        destination[5] = storedLat.u8[5];
        destination[6] = storedLat.u8[6];
        destination[7] = storedLat.u8[7];
        destination[8] = storedLng.u8[0];
        destination[9] = storedLng.u8[1];
        destination[10] = storedLng.u8[2];
        destination[11] = storedLng.u8[3];
        destination[12] = storedLng.u8[4];
        destination[13] = storedLng.u8[5];
        destination[14] = storedLng.u8[6];
        destination[15] = storedLng.u8[7];
    }

private:
    double_bytes storedLat;
    double_bytes storedLng;
};