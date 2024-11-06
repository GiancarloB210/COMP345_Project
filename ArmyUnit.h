#ifndef ARMYUNIT_H
#define ARMYUNIT_H
#include <ostream>

static int ArmyUnitID = 0;

class ArmyUnit {
public:
    int armyUnitID;

    ArmyUnit();
    ArmyUnit(ArmyUnit& other);
    ArmyUnit& operator=(ArmyUnit other);
    friend std::ostream& operator<<(std::ostream& os, const ArmyUnit& unit);
};

#endif //ARMYUNIT_H
