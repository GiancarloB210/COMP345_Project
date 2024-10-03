#ifndef CARD_H
#define CARD_H
#include <string>
#include <iostream>
#include <chrono>
#include "Orders.h"
#include <algorithm>

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
    bool availableToDraw; //Indicates whether the card can be drawn from its associated Deck (a means of establishing whether or not it's "in" the Deck).

    Card();
    Card(CardType* type);
    Card(Card& card);
    friend ostream& operator <<(ostream& out_stream, Card& card);
    friend istream& operator >>(istream& in_stream, Card& card);
    Card& operator=(Card card);
    void makeAvailableToDraw();
    void makeUnavailableToDraw();
    Order* play();
    std::string getCardTypeStringValue(); //Gets the corresponding enum value in string format.
    void clean();
};

class Deck {
public:
    Card* cards[40];

    Deck(); //Instantiates a Deck with 8 of each of the 5 types of cards.
    Deck(Card* newDeckCards[40]);
    Deck(Deck& deck);
    friend ostream& operator <<(ostream& out_stream, Deck& deck);
    friend istream& operator >>(istream& in_stream, Deck& deck);
    Deck& operator=(Deck deck);
    Card* draw();
    void clean();
};

class Hand {
public:
    Card* cardsInHand[40];
    Deck* deckPlayedWith;
    int* associatedDeckPointer;
    int* handPointer; //Points to the first free position in the hand (one without a card).

    Hand();
    Hand(Deck* deckInput);
    Hand(Hand& hand);
    friend ostream& operator <<(ostream& out_stream, Hand& hand);
    friend istream& operator >>(istream& in_stream, Hand& hand);
    Hand& operator=(Hand hand);
    Order* play(int cardIndex); //Simplifies the process of a player playing a card in their hand. Will call the corresponding card's play() method in order to abide by assignment requirements..
    void drawCard();
    void addToHand(Card* card);
    void displayCardsInHand();
    void clean(); //Removes any pointers to ensure no memory leakage.
};
#endif //CARD_H
