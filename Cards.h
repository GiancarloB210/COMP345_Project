#ifndef CARD_H
#define CARD_H
#include <string>
#include <iostream>

#include "Orders.h"

enum CardType {
    BOMB,
    REINFORCEMENT,
    BLOCKADE,
    AIRLIFT,
    DIPLOMACY
};

static int staticCardID = 0;

class Cards {
public:
    int16_t cardID;
    CardType* type;
    bool availableToDraw;

    Cards(CardType* type);
    Cards(Cards& card);
    void makeAvailableToDraw();
    void makeUnavailableToDraw();
    Order* play();
    std::string getCardTypeStringValue();
};

class Deck {
public:
    Cards* cards[40];

    Deck();
    Deck(Cards* newDeckCards[40]);
    Deck(Deck& deck);
    void shuffleDeck();
    Cards* draw();
};

class Hand {
public:
    Cards* cardsInHand[40];
    Deck* deckPlayedWith;
    int* associatedDeckPointer;
    int* handPointer;

    Hand(Deck* deckInput);
    Hand(Hand& hand);
    Order* play(int cardIndex);
    void addToHand(Cards* card);
    void displayCardsInHand();
    void clean();
};
#endif //CARD_H
