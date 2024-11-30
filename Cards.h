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
    ~Card();
    friend ostream& operator <<(ostream& out_stream, Card& card);
    friend istream& operator >>(istream& in_stream, Card& card);
    Card& operator=(Card card);
    void makeAvailableToDraw();
    void makeUnavailableToDraw();
    Order* play();
    std::string getCardTypeStringValue(); //Gets the corresponding enum value in string format.
};

class Deck {
public:
    std::vector<Card*> cards;

    Deck(); //Instantiates a Deck with 8 of each of the 5 types of cards.
    Deck(std::vector<Card*> cards);
    Deck(Deck& deck);
    ~Deck();
    friend ostream& operator <<(ostream& out_stream, Deck& deck);
    friend istream& operator >>(istream& in_stream, Deck& deck);
    Deck& operator=(Deck deck);
    Card* draw();
};

class Hand {
public:
    std::vector<Card*> cardsInHand;
    Deck* deckPlayedWith;
    int associatedDeckPosition;
    int handPosition;

    Hand();
    Hand(Deck* deckInput);
    Hand(Hand& hand);
    ~Hand();
    friend ostream& operator <<(ostream& out_stream, Hand& hand);
    friend istream& operator >>(istream& in_stream, Hand& hand);
    Hand& operator=(Hand hand);
    Order* play(int cardIndex); //Simplifies the process of a player playing a card in their hand. Will call the corresponding card's play() method in order to abide by assignment requirements..
    void drawCard();
    void addToHand(Card* card);
    void displayCardsInHand();
};
#endif //CARD_H
