#include "ArmyUnit.h"
#include "Player.h"

#include <iostream>
using namespace std;

//Default constructor.
ArmyUnit::ArmyUnit() {
    this->armyUnitID = ArmyUnitID;
    ArmyUnitID++;
}

//Constructor which takes a pointer to a game player as input.
ArmyUnit::ArmyUnit(Player* playerOwner) {
    this->armyUnitID = ArmyUnitID;
    ArmyUnitID++;
    this->playerOwner = playerOwner;
}

//Destructor.
ArmyUnit::~ArmyUnit() {
    delete this->playerOwner;
    playerOwner = NULL;
}

//Copy constructor.
ArmyUnit::ArmyUnit(ArmyUnit& otherUnit) {
    this->armyUnitID = otherUnit.armyUnitID;
    this->playerOwner = new Player(*otherUnit.playerOwner);
}

//Assignment operator.
ArmyUnit& ArmyUnit::operator=(ArmyUnit otherUnit) {
    if (this != &otherUnit) { // Self-assignment check
        swap(this->armyUnitID, otherUnit.armyUnitID);
        Player tempPlayerPointer = *(this->playerOwner);
        *(this->playerOwner) = *(otherUnit.playerOwner);
        *(otherUnit.playerOwner) = tempPlayerPointer;
    }
    return *this;
}

//Stream insertion operator.
ostream& operator<<(std::ostream& os, const ArmyUnit& unit) {
    os << "ArmyUnit with ID " << unit.armyUnitID << endl;
    return os;
}