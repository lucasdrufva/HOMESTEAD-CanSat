#pragma once

#include "log_level.h"

union float_bytes
{
    float flt;
    uint8_t u8[sizeof(float)];
};

union double_bytes
{
  double db;
  uint8_t u8[sizeof(double)];
};

enum DataType : uint8_t
{
  DATA_TYPE_TIME = 1,
  DATA_TYPE_TEMPERATURE = 2,
  DATA_TYPE_BATTERY = 3,
  DATA_TYPE_BME = 4,
  DATA_TYPE_UV = 5,
  DATA_TYPE_ACCELERATION = 6,
  DATA_TYPE_GYRO = 7,
  DATA_TYPE_MAGNET = 8,
  DATA_TYPE_ROTATION = 9,
  DATA_TYPE_ACTIVITY = 10,
  DATA_TYPE_STATUS = 11,
  DATA_TYPE_GPS = 12,
};

const uint8_t dataLength[256] = {
  0,
  4,
  4,
  4,
  16,
  8,
  12,
  12,
  12,
  16,
  1,
  1,
  16
};

class Data
{
public:
    virtual size_t getSize() = 0;
    virtual DataType getType() = 0;
    virtual void writeData(uint8_t *destination) = 0;

    LogLevel logLevel;
};