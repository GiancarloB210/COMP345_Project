#ifndef CARDSDRIVER_H
#define CARDSDRIVER_H
#include "Cards.h"

class CardsDriver {
public:
    CardsDriver();

    int countDrawableCards(Hand *hand);

    void testCards();
};

#endif //CARDSDRIVER_H
