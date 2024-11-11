#ifndef ARMYUNIT_H
#define ARMYUNIT_H

#include <ostream>

static int ArmyUnitID = 0;

class Player;

class ArmyUnit {
public:
    int armyUnitID;
    Player* playerOwner;

    ArmyUnit(); //Basic constructor.
    ArmyUnit(Player* player); //Basic constructor.
    ~ArmyUnit();
    ArmyUnit(ArmyUnit& other); //Copy constructor.
    ArmyUnit& operator=(ArmyUnit other); //Assignment operator.
    friend std::ostream& operator<<(std::ostream& os, const ArmyUnit& unit); //Stream insertion operator.
};

#endif //ARMYUNIT_H
