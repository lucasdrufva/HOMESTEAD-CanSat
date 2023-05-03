#pragma once

#include "unit.h"
#include "log_level.h"
#include "data.h"


class Logger: public Unit
{
public:
    void setLogLevel(LogLevel newLogLevel);
    void setTimeCallback(unsigned long (*cb)()){
        timeCallback = cb;
    }
    void logData(Data *data);

protected:
    LogLevel logLevel;
    unsigned long (*timeCallback)();
    void writeTime(uint8_t * dest);
};