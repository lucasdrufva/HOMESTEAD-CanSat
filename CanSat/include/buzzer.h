#pragma once

#include "unit.h"

class Buzzer: public Unit
{
public:
    bool begin(){ return true; }
    UnitStatus test(){ return UNIT_STATUS_OK; }
    String getName(){return "Camera";}
    void run();
    void indicateStatus(UnitStatus status);

private:
    UnitStatus currentStatus;
    unsigned long lastTone;
};