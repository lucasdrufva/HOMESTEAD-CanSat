#include "unit_controller.h"

bool UnitController::begin()
{
    for(auto unit : units)
    {
        Serial.print("Starting: ");
        Serial.println(unit->getName());
        if(!unit->begin())
        {
            // TODO change return value and report what device failed
            return false;
        }
    }
    return true;
}

UnitStatus UnitController::test()
{
    // TODO change return value and report what device failed
    UnitStatus lowestStatus = UNIT_STATUS_OK;

    for(auto unit : units)
    {
        UnitStatus unitStatus = unit->test();
        if(unitStatus < lowestStatus){
            lowestStatus = unitStatus;
        }
    }
    return lowestStatus;
}

void UnitController::run()
{
   for(auto unit : units)
    {
        //Serial.print("Unit: ");
        //Serial.println(unit->getName());
        unit->run();
    } 
}

void UnitController::addUnit(Unit *newUnit)
{
    units.push_back(newUnit);
}