#ifndef PLAYER_H
#define PLAYER_H
#include "Cards.h"
#include "Orders.h"

class Player {
public:
    OrdersList* orderList;
    Hand* playerHand;
    Player();
    Player(Player& player);
    Player& operator=(Player player);
    void issueOrder(std::string orderType);
    void playCard(int16_t targetCardID);
    void draw();
};

#endif //PLAYER_H
