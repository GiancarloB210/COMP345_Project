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
    friend ostream& operator <<(ostream& out_stream, Player& player);
    friend istream& operator >>(istream& in_stream, Player& player);
    Player& operator=(Player player);
    void issueOrder(std::string orderType);
    void playCard(int16_t targetCardID);
    void draw();
};

#endif //PLAYER_H
