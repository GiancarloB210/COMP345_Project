#ifndef CARD_H
#define CARD_H
#include <string>
#include <iostream>

#include "Orders.h"

using namespace std;

enum CardType {
    BOMB,
    REINFORCEMENT,
    BLOCKADE,
    AIRLIFT,
    DIPLOMACY
};

static int staticCardID = 0;

class Card {
public:
    int16_t cardID;
    CardType* type;
    bool availableToDraw;

    Card(CardType* type);
    Card(Card& card);
    friend ostream& operator <<(ostream& out_stream, Card& card);
    friend istream& operator >>(istream& in_stream, Card& card);
    Card& operator=(Card card);
    void makeAvailableToDraw();
    void makeUnavailableToDraw();
    Order* play();
    std::string getCardTypeStringValue();
};

class Deck {
public:
    Card* cards[40];

    Deck();
    Deck(Card* newDeckCards[40]);
    Deck(Deck& deck);
    friend ostream& operator <<(ostream& out_stream, Deck& deck);
    friend istream& operator >>(istream& in_stream, Deck& deck);
    Deck& operator=(Deck deck);
    void shuffleDeck();
    Card* draw();
};

class Hand {
public:
    Card* cardsInHand[40];
    Deck* deckPlayedWith;
    int* associatedDeckPointer;
    int* handPointer;

    Hand(Deck* deckInput);
    Hand(Hand& hand);
    friend ostream& operator <<(ostream& out_stream, Hand& hand);
    friend istream& operator >>(istream& in_stream, Hand& hand);
    Hand& operator=(Hand hand);
    Order* play(int cardIndex);
    void drawCard();
    void addToHand(Card* card);
    void displayCardsInHand();
    void clean();
};
#endif //CARD_H
