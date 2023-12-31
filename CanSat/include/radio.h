#pragma once

#include "logger.h"
#include "data.h"

#include <RH_RF95.h>
#include <RHSPIDriver.h>

#define RFM95_CS 21
//#define RFM95_CS 4
#define RFM95_RST 30
//#define RFM95_RST 2
#define RFM95_INT 20

class Radio: public Logger
{
public:
    Radio() : rf95(RFM95_CS, RFM95_INT) {};
    bool begin();
    UnitStatus test();
    String getName(){
        return "Radio";
    }

    void logData(Data *data);
    void run();

    void setDataCallback(void (*cb)(Data *data)) 
    {
        dataCallback = cb;
    }

private:
    void (*dataCallback)(Data *data);
    RH_RF95 rf95;
    void setSpiModeRadio();
    unsigned long lastBno;
};