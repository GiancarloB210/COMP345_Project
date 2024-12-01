#include "Player.h"
#include "Map.h"
#include "Cards.h"
#include <iostream>

// Function to test the Player class methods
void testPlayers() {
    // Mock territories from the Map
    Territory* t1 = new Territory("Territory1", "Continent1", 10, 10, std::vector<std::string>{"Territory2", "Territory3"});
    Territory* t2 = new Territory("Territory2", "Continent1", 20, 20, std::vector<std::string>{"Territory1", "Territory4"});
    Territory* t3 = new Territory("Territory3", "Continent1", 30, 30, std::vector<std::string>{"Territory1", "Territory5"});
    
    std::vector<Territory*> territories = { t1, t2, t3 };
    std::vector<Territory*>* territoryPointer = &territories;

    // Mock hand of cards and deck
    Deck* deck = new Deck();
    Hand* hand = new Hand(deck);

    // Create a player object
    Player* player = new Player(territoryPointer, hand);

    // Test toDefend method
    std::cout << "Testing toDefend method: \n";
    std::vector<Territory*> defendList = player->toDefend();
    for (Territory* t : defendList) {
        std::cout << "Territory to defend: " << t->getName() << "\n";
    }

    // Test toAttack method
    std::cout << "Testing toAttack method: \n";
    std::vector<Territory*> attackList = player->toAttack();
    for (Territory* t : attackList) {
        std::cout << "Territory to attack: " << t->getName() << "\n";
    }

    // Test issueOrder method
    std::cout << "Testing issueOrder method:\n";
    player->issueOrder();
    std::cout << "Player's orders: \n";
    std::cout << *(player->orders) << "\n";

    // Test stream insertion operator (printing player details)
    std::cout << "Testing stream insertion operator (Player details):\n";
    std::cout << *player;

    // Clean up dynamically allocated memory
    delete player;
    delete t1;
    delete t2;
    delete t3;
    delete deck;
}
