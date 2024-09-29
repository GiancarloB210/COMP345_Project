#include "Cards.h"
#include <iostream>

#include "Orders.h"
using namespace std;

Cards::Cards(CardType* type) {
    this->cardID = staticCardID;
    staticCardID++;
    this->type = type;
    this->availableToDraw = true;
}

Cards::Cards(Cards& card) {
    this->cardID = card.cardID;
    this->type = *(&card.type);
    this->availableToDraw = card.availableToDraw;
}

void Cards::makeAvailableToDraw() {
    this->availableToDraw = new bool(true);
}

void Cards::makeUnavailableToDraw() {
    this->availableToDraw = new bool(false);
}

Order* Cards::play() {
    switch(*type) {
        case BOMB:
            cout<<"Opposing territory bombed."<<endl;
            return new BombOrder(true);
        case REINFORCEMENT:
            cout<<"Calling for reinforcements."<<endl;
            return new OtherSpecialOrder("Reinforcement", true);
        case BLOCKADE:
            cout<<"Establishing a blockade."<<endl;
            return new BlockadeOrder(true);
        case AIRLIFT:
            cout<<"Calling for an airlift."<<endl;
            return new AirliftOrder(true);
        case DIPLOMACY:
            cout<<"Establishing a diplomacy."<<endl;
            return new OtherSpecialOrder("Diplomacy", true);
        default:
            cout<<"Invalid type."<<endl;
            return new OtherSpecialOrder("Invalid", false);
    }
}

std::string Cards::getCardTypeStringValue() {
    if (*this->type == BOMB) {
        return"BOMB";
    } else if (*this->type == REINFORCEMENT) {
        return"REINFORCEMENT";
    } else if (*this->type == BLOCKADE) {
        return"BLOCKADE";
    } else if (*this->type == AIRLIFT) {
        return"AIRLIFT";
    } else if (*this->type == DIPLOMACY) {
        return"DIPLOMACY";
    } else {
        return "Invalid card type.";
    }
}

Deck::Deck() {
    for (int i = 0; i < 8; i++) {
        this->cards[i] = new Cards(new CardType(BOMB));;
    }
    for (int i = 8; i < 16; i++) {
        this->cards[i] = new Cards(new CardType(REINFORCEMENT));
    }
    for (int i = 16; i < 24; i++) {
        this->cards[i] = new Cards(new CardType(BLOCKADE));
    }
    for (int i = 24; i < 32; i++) {
        this->cards[i] = new Cards(new CardType(AIRLIFT));
    }
    for (int i = 32; i < 40; i++) {
        this->cards[i] = new Cards(new CardType(DIPLOMACY));
    }
}

Deck::Deck(Cards* newDeckCards[40]) {
    for (int i = 0; i < 40; i++) {
        this->cards[i] = newDeckCards[i];
    }
}

Deck::Deck(Deck& deck) {
    for (int i = 0; i < 40; i++) {
        this->cards[i] = deck.cards[i];
    }
}

void Deck::shuffleDeck() {
    srand((int)time(0));
    string check[40];
    for (int i = 0; i < 40; i++) {
        check[i] = "Available";
    }
    Deck tempDeck = Deck();
    for (int i = 0; i < 40; i++) {
        int randomPosition = random() % 40;
        if (check[randomPosition] == "Available") {
            this->cards[i] = tempDeck.cards[randomPosition];
            check[randomPosition] = "Not Available";
        } else {
            while (check[randomPosition] != "Available") {
                randomPosition = (rand() % 40);
            }
            this->cards[i] = tempDeck.cards[randomPosition];
            check[randomPosition] = "Not Available";
        }
    }
}

Cards* Deck::draw() {
    this->shuffleDeck();
    int i = 0;
    if (this->cards[i]->availableToDraw == false) {
        i++;
        while (this->cards[i]->availableToDraw == false) {
            i++;
        }
    }
    this->cards[i]->availableToDraw = false;
    return this->cards[i];
}

// void Deck::draw() {
//     this->deckPlayedWith->shuffleDeck();
//     int i = 0;
//     if (*(deckPlayedWith->cards[i]->isAvailableToDraw()) == false) {
//         i++;
//         while (*(deckPlayedWith->cards[i]->isAvailableToDraw()) == false) {
//             i++;
//         }
//     }
//     if (this->cardsInHand[*handPointer] != nullptr) {
//         *handPointer = *handPointer + 1;
//         while (this->cardsInHand[*handPointer] != nullptr) {
//             *handPointer = *handPointer + 1;
//         }
//     }
//     this->cardsInHand[*handPointer] = deckPlayedWith->cards[i];
//     deckPlayedWith->cards[i]->makeUnavailableToDraw();
// }

Hand::Hand(Deck* deckInput) {
    this->deckPlayedWith = deckInput;
    this->associatedDeckPointer = new int(0);
    this->handPointer = new int(0);
}

Hand::Hand(Hand& hand) {
    for (int i = 0; i < 40; i++) {
        this->cardsInHand[i] = hand.cardsInHand[i];
    }
    this->deckPlayedWith = hand.deckPlayedWith;
    this->associatedDeckPointer = hand.associatedDeckPointer;
    this->handPointer = hand.handPointer;
}

Order* Hand::play(int cardIndex) {
    Order* order = this->cardsInHand[cardIndex]->play();
    for (int i = 0; i < 40; i++) {
        if (this->deckPlayedWith->cards[i]->cardID == this->cardsInHand[cardIndex]->cardID) {
            this->deckPlayedWith->cards[i]->makeAvailableToDraw();
            break;
        }
    }
    this->cardsInHand[cardIndex] = nullptr;
    *handPointer = cardIndex;
    return order;
}

void Hand::addToHand(Cards* card) {
    this->cardsInHand[*handPointer] = card;
    *handPointer = *(handPointer) + 1;
    while (this->cardsInHand[*handPointer] != nullptr) {
        *handPointer = *(handPointer) + 1;
    }
}

void Hand::displayCardsInHand() {
    for (int i = 0; i < 40; i++) {
        if (this->cardsInHand[i] == nullptr) {
            cout<<"No card in position "<<i<<endl;
        } else {
            cout<<i<<": "<<this->cardsInHand[i]->getCardTypeStringValue()<<endl;
        }
    }
}

void Hand::clean() {
    delete this->handPointer;
    delete this->associatedDeckPointer;
    for (int i = 0; i < 40; i++) {
        delete(cardsInHand[i]);
    }
}
