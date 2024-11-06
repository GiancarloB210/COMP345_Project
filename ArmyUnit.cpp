#include "ArmyUnit.h"

#include <iostream>
using namespace std;

ArmyUnit::ArmyUnit() {
    this->armyUnitID = ArmyUnitID;
    ArmyUnitID++;
}

ArmyUnit::ArmyUnit(ArmyUnit& other) {
    this->armyUnitID = other.armyUnitID;
}

ArmyUnit& ArmyUnit::operator=(ArmyUnit other) {
    if (this != &other) { // Self-assignment check
        swap(this->armyUnitID, other.armyUnitID);
    }
    return *this;
}

ostream& operator<<(std::ostream& os, const ArmyUnit& unit) {
    os << "ArmyUnit with ID " << unit.armyUnitID << endl;
}