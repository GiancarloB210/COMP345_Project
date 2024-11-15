#ifndef PLAYER_H
#define PLAYER_H

#include <list>
#include <string>
#include <vector>

#include "ArmyUnit.h"
#include "Map.h"    // For Territory
#include "Cards.h"  // For Hand
#include "Orders.h" // For OrdersList

class Territory;

static int staticPlayerID = 0;

class Player {
public:
    int playerID;
    std::string name;                   // Name of the player
    std::list<Territory*>* territories; // List of territories owned by the player
    Hand* hand;                         // Pointer to the player's hand of cards
    OrderList* orders;                  // Pointer to the player's list of orders
    std::vector<ArmyUnit*> armyUnits;   // Army units belonging to the player
    int reinforcementPool;              // Army units available for reinforcements

    // Constructors
    Player(std::string newName, std::list<Territory*>* territories, Hand* hand); // Parameterized constructor with name input
    Player(std::list<Territory*>* territories, Hand* hand);                      // Parameterized constructor without name input
    Player(const Player& player);                                                // Copy constructor

    // Destructor
    ~Player();

    // Assignment operator
    Player& operator=(const Player& player);

    // Stream insertion operator
    friend std::ostream& operator<<(std::ostream& out, const Player& player);

    // Methods
    std::list<Territory*>* toDefend();
    std::list<Territory*>* toAttack();
    void issueOrder();
    std::string getName() const;
    const std::list<Territory*>& getTerritories() const;

    // New methods to support game loop functionality
    void addToReinforcementPool(int units);     // Adds units to the reinforcement pool
    bool hasMoreOrders() const;                 // Checks if the player has more orders to issue/execute
    bool ownsTerritory() const;                 // Checks if the player owns any territory
    Order* getNextOrder();                      // Retrieves the next order to execute
};

#endif // PLAYER_H
