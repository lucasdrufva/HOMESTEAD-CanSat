#pragma once

#include "unit.h"
#include "data.h"

class Sensor: public Unit
{
public:
    void setDataCallback(void (*cb)(Data *data)) 
    {
        dataCallback = cb;
    }

    void setPeriod(unsigned long newPeriod) 
    {
        period = newPeriod;
    }
    

protected:
    void (*dataCallback)(Data *data);
    unsigned long period = 0;
    unsigned long lastUpdate = 0;
};