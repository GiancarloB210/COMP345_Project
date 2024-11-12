#include "Player.h"
#include <iostream>
#include <algorithm>

// Parameterized constructor (without name input)
Player::Player(std::list<Territory*>* territories, Hand* hand) {
    this->playerID = staticPlayerID;
    staticPlayerID++;
    this->name = "No Name";
    this->territories = new std::list<Territory*>(*territories);
    this->hand = new Hand(*hand);
    this->orders = new OrderList();
    this->armyUnits = std::vector<ArmyUnit*>();
    this->reinforcementPool = 0; // Initialize reinforcement pool
}

// Parameterized constructor (with name input)
Player::Player(std::string newName, std::list<Territory*>* territories, Hand* hand) {
    this->playerID = staticPlayerID;
    staticPlayerID++;
    this->name = newName;
    this->territories = new std::list<Territory*>(*territories);
    this->hand = new Hand(*hand);
    this->orders = new OrderList();
    this->armyUnits = std::vector<ArmyUnit*>();
    this->reinforcementPool = 0; // Initialize reinforcement pool
}

// Copy constructor (deep copy)
Player::Player(const Player& player) {
    this->playerID = staticPlayerID;
    staticPlayerID++;
    this->name = player.name;
    this->territories = new std::list<Territory*>(*player.territories);
    this->hand = new Hand(*player.hand);
    this->orders = new OrderList(*player.orders);
    this->armyUnits = std::vector<ArmyUnit*>(player.armyUnits);
    this->reinforcementPool = player.reinforcementPool;
}

// Destructor to clean up dynamic memory
Player::~Player() {
    delete this->territories;
    this->territories = NULL;
    delete this->hand;
    this->hand = NULL;
    delete this->orders;
    this->orders = NULL;
    for (int i = 0; i < this->armyUnits.size(); i++) {
        delete this->armyUnits[i];
        this->armyUnits[i] = NULL;
    }
    this->armyUnits.clear();
}

// Assignment operator (deep copy)
Player& Player::operator=(const Player& player) {
    if (this != &player) {
        // Clean up existing dynamic memory
        delete this->territories;
        delete this->hand;
        delete this->orders;
        this->armyUnits.clear();

        // Allocate new memory and copy the content
        this->playerID = player.playerID;
        this->name = player.name;
        this->territories = new std::list<Territory*>(*player.territories);
        this->hand = new Hand(*player.hand);
        this->orders = new OrderList(*player.orders);
        this->armyUnits = std::vector<ArmyUnit*>(player.armyUnits);
        this->reinforcementPool = player.reinforcementPool;
    }
    return *this;
}

// Stream insertion operator for printing player information
std::ostream& operator<<(std::ostream& out, const Player& player) {
    out << "Player's ID: " << player.playerID << std::endl;
    out << "Player's name: " << player.name << std::endl;
    out << "Player's territories to defend: ";
    for (Territory* t : *(player.territories)) {
        out << t->getName() << " ";
    }
    out << "\nPlayer's hand of cards: " << *(player.hand);
    out << "\nPlayer's list of orders: " << *(player.orders);
    out << "\nPlayer's number of army units: " << player.armyUnits.size();
    return out;
}

// Accessor methods
std::string Player::getName() const {
    return name;
}

const std::list<Territory*>& Player::getTerritories() const {
    return *territories;
}

// Method to return a list of all territories to defend (returns the pointer to the list)
std::list<Territory*>* Player::toDefend() {
    auto* defendList = new std::list<Territory*>;
    
    // Add all territories from the player's list of territories
    for (Territory* t : *territories) {
        defendList->push_back(t);
    }
    
    return defendList;
}

// Method to return a list of all territories to attack (returns the pointer to the list)
std::list<Territory*>* Player::toAttack() {
    auto* attackList = new std::list<Territory*>;

    // Reverse the list to simulate attacking from the opposite end of territories
    for (auto it = territories->rbegin(); it != territories->rend(); ++it) {
        attackList->push_back(*it);
    }

    return attackList;
}

// Method to issue an order and add it to the player's list of orders
void Player::issueOrder() {
    Order* newOrder = new DeployOrder();
    orders->add(newOrder);
    std::cout << "Issued a DeployOrder.\n";
}

// Adds units to the reinforcement pool
void Player::addToReinforcementPool(int units) {
    reinforcementPool += units;
}

// Checks if the player has more orders to issue or execute
bool Player::hasMoreOrders() const {
    return !orders->getList().empty();
}

// Checks if the player owns any territory
bool Player::ownsTerritory() const {
    return !territories->empty();
}

// Retrieves and removes the next order from the player's orders list
Order* Player::getNextOrder() {
    if (!orders->getList().empty()) {
        Order* nextOrder = orders->getList().front();
        orders->getList().erase(orders->getList().begin());
        return nextOrder;
    }
    return nullptr; // Return nullptr if there are no orders
}
