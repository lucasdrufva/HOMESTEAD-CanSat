#include "buzzer.h"

#include "pins.h"

void Buzzer::indicateStatus(UnitStatus status)
{
    currentStatus = status;
}

void Buzzer::run()
{
    if(currentStatus == UNIT_STATUS_LOCATE)
    {
        if(millis() - lastTone > 400){
            tone(PIN_BUZZER, 1000, 200);
        }
    }
    else{
        if(millis() - lastTone > 1000){
            tone(PIN_BUZZER, 400+(currentStatus*300), 200);
        } 
    }
}