#include "logger.h"

#include <Arduino.h>

void Logger::writeTime(uint8_t *destination)
{
    unsigned long time;
    if(timeCallback == nullptr)
        time = millis();
    else
        time = timeCallback();
    
    destination[0] = time & 0xff;
    destination[1] = (time>>8) & 0xff;
    destination[2] = (time>>16) & 0xff;
    destination[3] = (time>>24) & 0xff;
}