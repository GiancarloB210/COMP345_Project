#ifndef PLAYER_H
#define PLAYER_H

#include <list>
#include <string>
#include "Map.h"  
#include "Cards.h"
#include "Orders.h"

class Player {
public:
    std::list<Territory*> territories;  
    Hand* hand;                        
    OrdersList* orders;                  

    Player(std::list<Territory*> territories, Hand* hand);

    Player(Player& player);

    ~Player();

    std::list<Territory*> toDefend();

    std::list<Territory*> toAttack();

    void issueOrder();
};

#endif // PLAYER_H
