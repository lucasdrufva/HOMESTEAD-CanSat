#pragma once

#include "logger.h"

#include <SPIFlash.h> 

const uint8_t START_SYMBOL_U8[4] = {0xde, 0xad, 0xbe, 0xef};
const uint32_t START_SYMBOL_U32 = 0xefbeadde;

class Flash: public Logger
{
public:
    // TODO refactor constans
    Flash() : flash(28, 0xEF40) {}
    bool begin();
    UnitStatus test();
    String getName() { return "Flash"; }

    void logData(Data *data);

    void exportData();
private:
    SPIFlash flash;
    void setSpiModeFlash();
    uint32_t write_addr = 0;
    int page = -1;
    void printFlightData(uint32_t addr);
};
