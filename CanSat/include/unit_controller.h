#pragma once

#include "unit.h"
#include <vector>

#define MAX_UNIT_COUNT 16

class UnitController: Unit
{
public:
    bool begin();
    UnitStatus test();
    void run();
    void addUnit(Unit *newUnit);

private:
    std::vector<Unit*> units;
};