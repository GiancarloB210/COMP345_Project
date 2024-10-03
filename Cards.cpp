#include "Cards.h"
#include <iostream>

#include "Orders.h"
using namespace std;

Card::Card(CardType* type) {
    this->cardID = staticCardID;
    staticCardID++;
    this->type = type;
    this->availableToDraw = true;
}

Card::Card(Card& card) {
    this->cardID = card.cardID;
    this->type = *(&card.type);
    this->availableToDraw = card.availableToDraw;
}

Card& Card::operator=(Card card) {
    swap(this->cardID, card.cardID);
    CardType tempTypePointer = *(this->type);
    *(this->type) = *(card.type);
    *(card.type) = tempTypePointer;
    swap(this->availableToDraw, card.availableToDraw);
    return *this;
}

ostream& operator << (ostream &out_stream, Card &card) {
    out_stream<<"ID: "<<card.cardID<<" | Type: "<<to_string(*(card.type))<<" | Available to draw: "<<(card.availableToDraw ? "Yes" : "No")<<endl;
    return out_stream;
}

istream& operator >> (istream &in_stream, Card &card) {
    cout<<"What type of card is this?"<<endl;
    string input;
    in_stream>>input;
    for (char& ch : input) {
        ch = tolower(ch, locale());
    }
    if (input == "bomb") {
        card.type = new CardType(BOMB);
    }
    else if (input == "reinforcement") {
        card.type = new CardType(REINFORCEMENT);
    }
    else if (input == "airlift") {
        card.type = new CardType(AIRLIFT);
    }
    else if (input == "blockade") {
        card.type = new CardType(BLOCKADE);
    }
    else if (input == "diplomacy") {
        card.type = new CardType(DIPLOMACY);
    }
    else {
        throw std::invalid_argument("Invalid card type entered");
    }
    return in_stream;
}

void Card::makeAvailableToDraw() {
    this->availableToDraw = true;
}

void Card::makeUnavailableToDraw() {
    this->availableToDraw = false;;
}

Order* Card::play() {
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

std::string Card::getCardTypeStringValue() {
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
        this->cards[i] = new Card(new CardType(BOMB));;
    }
    for (int i = 8; i < 16; i++) {
        this->cards[i] = new Card(new CardType(REINFORCEMENT));
    }
    for (int i = 16; i < 24; i++) {
        this->cards[i] = new Card(new CardType(BLOCKADE));
    }
    for (int i = 24; i < 32; i++) {
        this->cards[i] = new Card(new CardType(AIRLIFT));
    }
    for (int i = 32; i < 40; i++) {
        this->cards[i] = new Card(new CardType(DIPLOMACY));
    }
}

Deck::Deck(Card* newDeckCards[40]) {
    for (int i = 0; i < 40; i++) {
        this->cards[i] = newDeckCards[i];
    }
}

Deck::Deck(Deck& deck) {
    for (int i = 0; i < 40; i++) {
        this->cards[i] = deck.cards[i];
    }
}

ostream& operator << (ostream &out_stream, Deck &deck) {
    out_stream<<"Deck Contents:"<<endl;
    for (int i = 0; i < 40; i++) {
        out_stream<<"["<<i<<"]: "<<to_string(*(deck.cards[i]->type))<<endl;
    }
    return out_stream;
}

istream& operator >> (istream &in_stream, Deck &deck) {
    Card* newCards[40];
    int howeverManyCards = 0;
    int lastPositionTracker = 0;
    cout<<"How many cards of type 'Bomb' would you like [7-9]?"<<endl;
    int numBombCards;
    in_stream>>numBombCards;
    while (numBombCards < 7 || numBombCards > 9) {
        cout<<"Invalid value. Enter a number between 7-9";
        in_stream>>numBombCards;
    }
    howeverManyCards += numBombCards;
    for (int i = lastPositionTracker;i < numBombCards;i++) {
        newCards[i] = new Card(new CardType(BOMB));
    }
    lastPositionTracker += numBombCards;
    cout<<"How many cards of type 'Reinforcement' would you like [7-9]?"<<endl;
    int numReinCards;
    in_stream>>numReinCards;
    while (numReinCards < 7 || numReinCards > 9) {
        cout<<"Invalid value. Enter a number between 7-9";
        in_stream>>numReinCards;
    }
    howeverManyCards += numReinCards;
    for (int i = lastPositionTracker;i < numReinCards;i++) {
        newCards[i] = new Card(new CardType(REINFORCEMENT));
    }
    lastPositionTracker += numReinCards;
    cout<<"How many cards of type 'Blockade' would you like [7-9]?"<<endl;
    int numBlockCards;
    in_stream>>numBlockCards;
    while (numBlockCards < 7 || numBlockCards > 9) {
        cout<<"Invalid value. Enter a number between 7-9";
        in_stream>>numBlockCards;
    }
    howeverManyCards += numBlockCards;
    for (int i = lastPositionTracker;i < numBlockCards;i++) {
        newCards[i] = new Card(new CardType(BLOCKADE));
    }
    lastPositionTracker += numBlockCards;
    cout<<"How many cards of type 'Airlift' would you like [7-9]?"<<endl;
    int numAirCards;
    in_stream>>numAirCards;
    while (numAirCards < 7 || numAirCards > 9) {
        cout<<"Invalid value. Enter a number between 7-9";
        in_stream>>numAirCards;
    }
    howeverManyCards += numAirCards;
    for (int i = lastPositionTracker;i < numAirCards;i++) {
        newCards[i] = new Card(new CardType(AIRLIFT));
    }
    lastPositionTracker += numAirCards;
    cout<<"The remaining "<<40 - howeverManyCards<<" cards will be of type 'Diplomacy'.";
    for (int i = lastPositionTracker;i < 40;i++) {
        newCards[i] = new Card(new CardType(DIPLOMACY));
    }
    for (int i = 0;i < 40;i++) {
        deck.cards[i] = newCards[i];
    }
    return in_stream;
}

Deck& Deck::operator=(Deck deck) {
    Card* tempDeckPointer = *(this->cards);
    *(this->cards) = *(deck.cards);
    *(deck.cards) = tempDeckPointer;
    return *this;
}

void Deck::shuffleDeck() {
    srand((int)time(0));
    string check[40];
    for (int i = 0; i < 40; i++) {
        check[i] = "Available";
    }
    Deck tempDeck;
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

Card* Deck::draw() {
    //The deck is shuffled twice such that a "random" card is drawn.
    this->shuffleDeck();
    this->shuffleDeck();
    for (int i = 0; i < 40; i++) {
        if (this->cards[i]->availableToDraw) {
            this->cards[i]->availableToDraw = false;
            return this->cards[i];
        }
    }
    throw runtime_error("No card in the deck is available to draw.");
}

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

ostream& operator << (ostream &out_stream, Hand &hand) {
    cout<<"Hand Contents:"<<endl;
    for (int i = 0; i < 40; i++) {
        if (hand.cardsInHand[i] != nullptr) {
            out_stream<<"["<<i<<"]: "<<to_string(*(hand.cardsInHand[i]->type))<<endl;
        }
    }
    return out_stream;
}

istream& operator >> (istream &in_stream, Hand &hand) {
    cout<<"How many cards would you like in this hand?"<<endl;
    int numCards;
    in_stream>>numCards;
    for (int i = 0; i < numCards; i++) {
        cout<<"What type of card would you like at hand position "<<i<<"?"<<endl;
        bool isValidType = false;
        string newType;
        while (!isValidType) {
            in_stream>>newType;
            for (char& ch : newType) {
                ch = tolower(ch);
            }
            if (newType == "bomb") {
                hand.cardsInHand[i] = new Card(new CardType(BOMB));
                isValidType = true;
            } else if (newType == "reinforcement") {
                    hand.cardsInHand[i] = new Card(new CardType(REINFORCEMENT));
                    isValidType = true;
            } else if (newType == "airlift") {
                    hand.cardsInHand[i] = new Card(new CardType(AIRLIFT));
                    isValidType = true;
            } else if (newType == "airlift") {
                    hand.cardsInHand[i] = new Card(new CardType(BLOCKADE));
                    isValidType = true;
            } else if (newType == "diplomacy") {
                    hand.cardsInHand[i] = new Card(new CardType(DIPLOMACY));
                    isValidType = true;
            } else {
                    cout<<"Invalid card type. Please enter a valid card type."<<endl;
            }
        }
    }
    return in_stream;
}

Hand& Hand::operator=(Hand hand) {
    Card* tempCardsInHandPointer = *(this->cardsInHand);
    *(this->cardsInHand) = *(hand.cardsInHand);
    *(hand.cardsInHand) = tempCardsInHandPointer;

    Deck tempDeckPlayedWithPointer = *(this->deckPlayedWith);
    *(this->deckPlayedWith) = *(hand.deckPlayedWith);
    *(hand.deckPlayedWith) = tempDeckPlayedWithPointer;

    int tempDeckPointer = *(this->associatedDeckPointer);
    *(this->associatedDeckPointer) = *(hand.associatedDeckPointer);
    *(hand.associatedDeckPointer) = tempDeckPointer;

    int tempHandPointer = *(this->handPointer);
    *(this->handPointer) = *(hand.handPointer);
    *(hand.handPointer) = tempHandPointer;

    return *this;
}

Order* Hand::play(int cardIndex) {
    Order* order = this->cardsInHand[cardIndex]->play();
    for (int i = 0; i < 40; i++) {
        if (this->deckPlayedWith->cards[i]->cardID == this->cardsInHand[cardIndex]->cardID) {
            //"Returns" the card to the deck by making it available to draw in the deck.
            this->deckPlayedWith->cards[i]->makeAvailableToDraw();
            break;
        }
    }
    //Removes the card from the hand, and sets the next empty spot in the hand to where the most recently played card originally was.
    this->cardsInHand[cardIndex] = nullptr;
    *handPointer = cardIndex;
    return order;
}

void Hand::addToHand(Card* card) {
    this->cardsInHand[*handPointer] = card;
    *handPointer = *(handPointer) + 1;
    while (this->cardsInHand[*handPointer] != nullptr) {
        *handPointer = *(handPointer) + 1;
    }
}

void Hand::drawCard() {
    this->cardsInHand[*handPointer] = this->deckPlayedWith->draw();
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