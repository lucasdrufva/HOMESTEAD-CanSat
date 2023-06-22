#pragma once

#include "unit.h"

class Camera: public Unit
{
public:
    bool begin();
    UnitStatus test(){ return UNIT_STATUS_OK; }
    String getName(){return "Camera";}
    void run();
private:
    bool started = false;
    unsigned long lastPic;
    unsigned long waitTime = 1000;
};