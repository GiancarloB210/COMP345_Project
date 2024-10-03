#ifndef CARDSDRIVER_H
#define CARDSDRIVER_H
#include "Cards.h"

class CardsDriver {
public:
    CardsDriver();
    CardsDriver(CardsDriver& driver);
    friend ostream& operator <<(ostream& out_stream, CardsDriver& driver);
    friend istream& operator >>(istream& in_stream, CardsDriver& driver);
    CardsDriver& operator=(CardsDriver driver);

    int countDrawableCards(Hand *hand);

    void testCards();
};

#endif //CARDSDRIVER_H
