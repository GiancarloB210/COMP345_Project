#include "Player.h"
#include "Map.h" 
#include "Cards.h"
#include <iostream>

void testPlayers() {
    Territory* t1 = new Territory("Territory1", "Continent1", 10, 10, std::list<std::string>{"Territory2", "Territory3"});
    Territory* t2 = new Territory("Territory2", "Continent1", 20, 20, std::list<std::string>{"Territory1", "Territory4"});
    Territory* t3 = new Territory("Territory3", "Continent1", 30, 30, std::list<std::string>{"Territory1", "Territory5"});
    
    std::list<Territory*> territories = { t1, t2, t3 };
    
    Deck* deck = new Deck();
    Hand* hand = new Hand(deck);
    
    Player* player = new Player(territories, hand);
    
    std::cout << "Territories to defend: \n";
    for (Territory* t : player->toDefend()) {
        std::cout << t->getName() << "\n";
    }
    
    std::cout << "Territories to attack: \n";
    for (Territory* t : player->toAttack()) {
        std::cout << t->getName() << "\n";
    }
    
    player->issueOrder();
    
    delete player;
    delete t1;
    delete t2;
    delete t3;
}

int main() {
    testPlayers();
    return 0;
}
