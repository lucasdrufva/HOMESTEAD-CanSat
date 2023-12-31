#pragma once

#include <Arduino.h>

enum UnitStatus 
{
  UNIT_STATUS_ERROR = 0,
  UNIT_STATUS_WARNING = 1,
  UNIT_STATUS_OK = 2,
  UNIT_STATUS_LOCATE = 3,
};

class Unit
{
public:
    virtual bool begin(){}
    virtual UnitStatus test(){}
    virtual String getName(){return "Unit";}
    virtual void run(){}
};


// Just somthing everyone needs
// TODO should not be here, find somewhere else
enum Mode {
  MODE_PAD = 0,
  MODE_FLIGHT = 1,
  MODE_RETRIEVAL = 2,
  MODE_EXPORT = 3
};