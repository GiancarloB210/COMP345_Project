#ifndef PLAYER_H
#define PLAYER_H

#include <list>
#include <string>
#include <vector>

class GameEngine;
class PlayerStrategy;
#include "ArmyUnit.h"
#include "Map.h"    // For Territory
#include "Cards.h"  // For Hand
#include "Orders.h" // For OrdersList
#include "GameEngine.h"
#include "PlayerStrategies.h"

class Territory;

static int staticPlayerID = 0;

struct DeployOrderInfo {
    Territory* territory;
    int numUnits;
};

struct TempTerritoryUnitInfo {
    Territory* territory;
    int numUnitsThere;
};

class Player {
public:
    int playerID;
    std::string name;                   // Name of the player
    std::vector<Territory*>* territories; // List of territories owned by the player
    Hand* hand;                         // Pointer to the player's hand of cards
    OrderList* orders;                  // Pointer to the player's list of orders
    std::vector<ArmyUnit*> armyUnits;   // Army units belonging to the player
    int reinforcementPool;              // Army units available for reinforcements
    GameEngine* currentGame;            //The game which the player is a part of.
    PlayerStrategy* ps;           //The type of the current player.

    // Constructors
    Player(std::string newName, std::vector<Territory*>* territories, Hand* hand); // Parameterized constructor with name input
    Player(std::vector<Territory*>* territories, Hand* hand);                      // Parameterized constructor without name input
    Player(const Player& player);                                                // Copy constructor

    // Destructor
    ~Player();

    // Assignment operator
    Player& operator=(const Player& player);

    // Stream insertion operator
    friend std::ostream& operator<<(std::ostream& out, const Player& player);

    // Methods
    // std::list<Territory*>* toDefend();
    // std::list<Territory*>* toAttack();
    std::vector<Territory*> toDefend();
    std::vector<Territory*> toAttack();
    void playCard(int handIndex);
    void issueOrder();
    std::string getName() const;
    const std::vector<Territory*>& getTerritories() const;

    // New methods to support game loop functionality
    void addToReinforcementPool(int units);     // Adds units to the reinforcement pool
    bool hasMoreOrders() const;                 // Checks if the player has more orders to issue/execute
    bool ownsTerritory() const;                 // Checks if the player owns any territory
    Order* getNextOrder();                      // Retrieves the next order to execute
    std::vector<TempTerritoryUnitInfo> getTempTerritoryUnitInfo(); //Returns information about territories and the number of units within them.
};

#endif // PLAYER_H
