#ifndef PLAYERSTRATEGY_H
#define PLAYERSTRATEGY_H
#include "Player.h"


class PlayerStrategy {
public:
    Player* pi;
    PlayerStrategy(Player* pi);

    virtual void issueOrder() = 0;
    virtual std::vector<Territory*> toAttack() = 0;
    virtual std::vector<Territory*> toDefend() = 0;
};

class HumanPlayerStrategy: public PlayerStrategy {
public:
    Player* pi;
    HumanPlayerStrategy(Player* pi);

    void issueOrder();
    std::vector<Territory*> toAttack();
    std::vector<Territory*> toDefend();
};

class AggressivePlayerStrategy: public PlayerStrategy {
public:
    Player* pi;
    AggressivePlayerStrategy(Player* pi);

    void issueOrder();
    std::vector<Territory*> toAttack();
    std::vector<Territory*> toDefend();
};

class BenevolentPlayerStrategy: public PlayerStrategy {
public:
    Player* pi;
    BenevolentPlayerStrategy(Player* pi);

    void issueOrder();
    std::vector<Territory*> toAttack();
    std::vector<Territory*> toDefend();
};



#endif //PLAYERSTRATEGY_H
