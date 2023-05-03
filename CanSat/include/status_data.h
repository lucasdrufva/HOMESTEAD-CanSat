#pragma once

#include "data.h"

enum StatusType : uint8_t
{
  STATUS_TYPE_START = 1,
  STATUS_TYPE_MODE_PAD = 2,
  STATUS_TYPE_MODE_FLIGHT = 3,
  STATUS_TYPE_MODE_DROP = 4,
  STATUS_TYPE_MODE_RETRIEVAL = 5,
};

class StatusData: public Data
{
public:
    StatusData(StatusType status)
    {
        storedStatus = status;
    };
    size_t getSize()
    {
        return 1;
    }
    DataType getType()
    {
        return DATA_TYPE_STATUS;
    }

    void writeData(uint8_t *destination)
    {
        destination[0] = storedStatus;
    };
private:
    StatusType storedStatus;
};