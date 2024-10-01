#include "Player.h"
#include <iostream>
#include <algorithm>

Player::Player(std::list<Territory*> territories, Hand* hand) {
    this->territories = territories;
    this->hand = hand;
    this->orders = new OrdersList();
}

Player::Player(Player& player) {
    this->territories = player.territories;
    this->hand = player.hand;
    this->orders = player.orders;
}

Player::~Player() {
    delete this->orders;
}

std::list<Territory*> Player::toDefend() {
    std::list<Territory*> defendList;
    int counter = 0;
    for (Territory* t : territories) {
        defendList.push_back(t);
        if (++counter >= 3) break; 
    }
    return defendList;
}

std::list<Territory*> Player::toAttack() {
    std::list<Territory*> attackList;
    int counter = 0;
    for (auto it = territories.rbegin(); it != territories.rend(); ++it) {
        attackList.push_back(*it);
        if (++counter >= 3) break;  
    }
    return attackList;
}

void Player::issueOrder() {
    Order* newOrder = new DeployOrder(true);
    this->orders->add(newOrder);
    std::cout << "Issued a DeployOrder.\n";
}
