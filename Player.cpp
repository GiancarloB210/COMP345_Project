#include "Player.h"
#include <iostream>
#include <algorithm>

// Parameterized constructor
Player::Player(std::list<Territory*>* territories, Hand* hand) {
    this->territories = new std::list<Territory*>(*territories);
    this->hand = new Hand(*hand);
    this->orders = new OrderList();
}

// Copy constructor (deep copy)
Player::Player(const Player& player) {
    this->territories = new std::list<Territory*>(*player.territories);
    this->hand = new Hand(*player.hand);
    this->orders = new OrderList(*player.orders);
}

// Destructor to clean up dynamic memory
Player::~Player() {
    delete territories;
    delete hand;
    delete orders;
}

// Assignment operator (deep copy)
Player& Player::operator=(const Player& player) {
    if (this != &player) {
        // Clean up existing dynamic memory
        delete territories;
        delete hand;
        delete orders;

        // Allocate new memory and copy the content
        this->territories = new std::list<Territory*>(*player.territories);
        this->hand = new Hand(*player.hand);
        this->orders = new OrderList(*player.orders);
    }
    return *this;
}

// Stream insertion operator for printing player information
std::ostream& operator<<(std::ostream& out, const Player& player) {
    out << "Player's territories to defend: ";
    for (Territory* t : *(player.territories)) {
        out << t->getName() << " ";
    }
    out << "\nPlayer's hand of cards: " << *(player.hand);
    out << "\nPlayer's list of orders: " << *(player.orders);
    return out;
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
