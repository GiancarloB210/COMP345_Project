#include "ArmyUnit.h"

#include <iostream>
using namespace std;

//Default constructor.
ArmyUnit::ArmyUnit() {
    this->armyUnitID = ArmyUnitID;
    ArmyUnitID++;
}

//Copy constructor.
ArmyUnit::ArmyUnit(ArmyUnit& other) {
    this->armyUnitID = other.armyUnitID;
}

//Assignment operator.
ArmyUnit& ArmyUnit::operator=(ArmyUnit other) {
    if (this != &other) { // Self-assignment check
        swap(this->armyUnitID, other.armyUnitID);
    }
    return *this;
}

//Stream insertion operator.
ostream& operator<<(std::ostream& os, const ArmyUnit& unit) {
    os << "ArmyUnit with ID " << unit.armyUnitID << endl;
    return os;
}