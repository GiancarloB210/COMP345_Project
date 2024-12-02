#include "Cards.h"
#include <iostream>
#include <random>

using namespace std;

//Default constructor. This should never be used.
Card::Card() {
    this->cardID = staticCardID;
    staticCardID++;
    this->type = nullptr;
    this->availableToDraw = false;
}

//This constructor should generally always be used when not copying one Card variable to another.
Card::Card(CardType* type) {
    //Using a static variable for the card ID serves as a substitution for an automatically incrementing variable (which this will essentially serve as).
    this->cardID = staticCardID;
    staticCardID++;
    this->type = type;
    this->availableToDraw = true;
}

//Copy constructor for the Hand class.
Card::Card(Card& card) {
    this->cardID = card.cardID;
    this->type = *(&card.type);
    this->availableToDraw = card.availableToDraw;
}

Card::~Card() {
    delete this->type;
    this->type = NULL;
}

Card& Card::operator=(Card card) {
    swap(this->cardID, card.cardID);
    //Swapping pointers requires storing one swapped value in a temporary value which can then be pointed to by the other swapped value.
    CardType tempTypePointer = *(this->type);
    *(this->type) = *(card.type);
    *(card.type) = tempTypePointer;
    swap(this->availableToDraw, card.availableToDraw);
    return *this;
}

//Displays all important card information.
ostream& operator << (ostream &out_stream, Card &card) {
    out_stream<<"ID: "<<card.cardID<<" | Type: "<<to_string(*(card.type))<<" | Available to draw: "<<(card.availableToDraw ? "Yes" : "No")<<endl;
    return out_stream;
}

//Allows for the creation of a new card with an inputted type via the console.
istream& operator >> (istream &in_stream, Card &card) {
    cout<<"What type of card is this?"<<endl;
    string input;
    in_stream>>input;
    //Converts the string into lowercase characters for ease of comparison in the lengthy if-else statement.
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
    //Only one of the 5 card types should be able to be played.
    else {
        throw std::invalid_argument("Invalid card type entered");
    }
    return in_stream;
}

//These two methods need not be used, since the availableToDraw property is public.
//They're just here if ever.
void Card::makeAvailableToDraw() {
    this->availableToDraw = true;
}

void Card::makeUnavailableToDraw() {
    this->availableToDraw = false;;
}

//Since no game-related functionality is required as of yet,
//a corresponding String is printed whenever a card is played.
//In addition, a corresponding pointer to an Order is returned,
//which aligns with the requirement of a card creating a corresponding
//order when it is played.
Order* Card::play() {
    switch(*type) {
        case BOMB:
            cout<<"Opposing territory bombed."<<endl;
            return new BombOrder();
        case REINFORCEMENT:
            cout<<"Calling for reinforcements."<<endl;
            return new DeployOrder();
        case BLOCKADE:
            cout<<"Establishing a blockade."<<endl;
            return new BlockadeOrder();
        case AIRLIFT:
            cout<<"Calling for an airlift."<<endl;
            return new AirliftOrder();
        case DIPLOMACY:
            cout<<"Establishing a diplomacy."<<endl;
            return new NegotiateOrder();
        default:
            cout<<"Invalid type."<<endl;
            throw runtime_error("Invalid card type entered");
    }
}

//Essentially converts a CardType enum to a string.
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

//Instantiates a Deck with 40 cards, 8 of each defined type.
Deck::Deck() {
    for (int i = 0; i < 8; i++) {
        this->cards.push_back(new Card(new CardType(BOMB)));
    }
    for (int i = 8; i < 16; i++) {
        this->cards.push_back( new Card(new CardType(REINFORCEMENT)));
    }
    for (int i = 16; i < 24; i++) {
        this->cards.push_back(new Card(new CardType(BLOCKADE)));
    }
    for (int i = 24; i < 32; i++) {
        this->cards.push_back(new Card(new CardType(AIRLIFT)));
    }
    for (int i = 32; i < 40; i++) {
        this->cards.push_back(new Card(new CardType(DIPLOMACY)));
    }
}

//Instantiates a Deck with 40 cards from a pre-made array.
Deck::Deck(std::vector<Card*> preMadeDeck) {
    this->cards = preMadeDeck;
}

//Copy constructor for the Deck class.
Deck::Deck(Deck& deck) {
    this->cards = deck.cards;
}

Deck::~Deck() {
    int size = this->cards.size();
    for (int i = 0; i < size; i++) {
        delete this->cards[i];
        this->cards[i] = NULL;
    }
}

//Returns the number of cards in the deck which can still be drawn.
int Deck::getNumDrawableCards() {
    int numDrawable = 0;
    for (int i = 0;i < this->cards.size();i++) {
        if (this->cards[i]->availableToDraw == true) {
            numDrawable++;
        }
    }
    return numDrawable;
}

//Outputs all deck contents (all cards in the Deck).
ostream& operator << (ostream &out_stream, Deck &deck) {
    out_stream<<"Deck Contents:"<<endl;
    for (int i = 0; i < 40; i++) {
        out_stream<<*(deck.cards[i]);
    }
    return out_stream;
}

//Allows for the creation of a new Deck via console input.
istream& operator >> (istream &in_stream, Deck &deck) {
    std::vector<Card*> newCards;
    //How many cards are in the Deck at any time. Needed for ensuring that exactly 40 cards are added.
    int howeverManyCards = 0;
    //Keeps track of the position of the last card in the Deck. Needed for adding more cards afterwards without any gaps.
    int lastPositionTracker = 0;
    cout<<"How many cards of type 'Bomb' would you like [7-9]?"<<endl;
    int numBombCards;
    in_stream>>numBombCards;
    //For the sake of ensuring a fair or semi-fair distribution, 7 to 9 cards per type will be allowed such that there is no over-the-top excess of one card type compared to another (with extreme 7-7-7-7-12 and 9-9-9-9-4 splits).
    while (numBombCards < 7 || numBombCards > 9) {
        cout<<"Invalid value. Enter a number between 7-9";
        in_stream>>numBombCards;
    }
    //Adds the number of inserted cards to the total number of cards in the Deck.
    howeverManyCards += numBombCards;
    //Instantiates the inputted number of cards within the Deck array, starting from the position of the "last" card in the array..
    for (int i = lastPositionTracker;i < numBombCards;i++) {
        newCards.push_back(new Card(new CardType(BOMB)));
    }
    //Updates the position of the last card position accordingly.
    lastPositionTracker += numBombCards;
    //The above comments hold true for the following 4 repetitions of the above code, only for the other 4 card types).
    cout<<"How many cards of type 'Reinforcement' would you like [7-9]?"<<endl;
    int numReinCards;
    in_stream>>numReinCards;
    while (numReinCards < 7 || numReinCards > 9) {
        cout<<"Invalid value. Enter a number between 7-9";
        in_stream>>numReinCards;
    }
    howeverManyCards += numReinCards;
    for (int i = lastPositionTracker;i < numReinCards;i++) {
        newCards.push_back(new Card(new CardType(REINFORCEMENT)));
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
        newCards.push_back(newCards[i] = new Card(new CardType(BLOCKADE)));
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
        newCards.push_back(newCards[i] = new Card(new CardType(AIRLIFT)));
    }
    lastPositionTracker += numAirCards;
    cout<<"The remaining "<<40 - howeverManyCards<<" cards will be of type 'Diplomacy'.";
    for (int i = lastPositionTracker;i < 40;i++) {
        newCards.push_back(newCards[i] = new Card(new CardType(DIPLOMACY)));
    }
    //Adds all of the cards in the temporary Deck to the Deck variable we wish to create.
    deck.cards = newCards;
    return in_stream;
}

Deck& Deck::operator=(Deck deck) {
    //Swapping pointers requires storing one swapped value in a temporary value which can then be pointed to by the other swapped value.
    for (int i = 0; i < this->cards.size(); i++) {
        Card* tempDeckPointer = this->cards[i];
        this->cards[i] = deck.cards[i];
        deck.cards[i] = tempDeckPointer;
    }
    return *this;
}

Card* Deck::draw() {
    //The deck is shuffled such that a "random" card is drawn.
    //Code for shuffling an array and obtaining a true random generation using a time-oriented seed sourced from: https://cplusplus.com/reference/algorithm/shuffle/
    shuffle(std::begin(this->cards), std::end(this->cards), std::default_random_engine(std::chrono::system_clock::now().time_since_epoch().count()));
    //Returns the first card in the Deck that can actually be drawn (the first card in what would be a tangible Deck with cards already drawn),
    for (int i = 0; i < this->cards.size(); i++) {
        if (this->cards[i]->availableToDraw == true) {
            this->cards[i]->makeUnavailableToDraw();
            return this->cards[i];
        }
    }
    throw runtime_error("No card in the deck is available to draw.");
}

//Default constructor. Seeing as how all hands should be associated with a pre-initialized deck, this should never be used.
Hand::Hand() {
    this->deckPlayedWith = new Deck();
    this->associatedDeckPosition = 0;
    this->handPosition = 0;
}

//This constructor, aside from in instances of wanting to copy one Hand variable to another, should not be used,
Hand::Hand(Deck* deckInput) {
    this->deckPlayedWith = deckInput;
    //int constructors are used to ensure that the integer pointers function properly.
    this->associatedDeckPosition = 0;
    this->handPosition = 0;
}

//Copy constructor for the Hand class.
Hand::Hand(Hand& hand) {
    //The 40 pointers in the pointer array are individually copied, since the entire pointer array cannot be.
    this->cardsInHand = hand.cardsInHand;
    this->deckPlayedWith = hand.deckPlayedWith;
    this->associatedDeckPosition = hand.associatedDeckPosition;
    this->handPosition = hand.handPosition;
}

Hand::~Hand() {
    //The deck pointer will already have been deleted by the time this destructor is called.
}

//Displays all cards in the current hand, at their respective indexes.
ostream& operator << (ostream &out_stream, Hand &hand) {
    cout<<"Hand Contents:"<<endl;
    for (int i = 0; i < 40; i++) {
        //If no card is in the hand at the current position, don't display anything.
        if (hand.cardsInHand[i] != nullptr) {
            out_stream<<"["<<i<<"]: "<<to_string(*(hand.cardsInHand[i]->type))<<endl;
        }
    }
    return out_stream;
}

//Instantiates cards in a hand which are created via the console.
istream& operator >> (istream &in_stream, Hand &hand) {
    cout<<"How many cards would you like in this hand?"<<endl;
    int numCards;
    in_stream>>numCards;
    for (int i = 0; i < numCards; i++) {
        //Calls the overridden stream insertion operator for the Card class.
        in_stream>>*(hand.cardsInHand[i]);
    }
    return in_stream;
}

Hand& Hand::operator=(Hand hand) {
    //Swapping pointers requires storing one swapped value in a temporary value which can then be pointed to by the other swapped value.
    for (int i = 0; i < this->cardsInHand.size(); i++) {
        Card* tempCardsInHandPointer = this->cardsInHand[i];
        this->cardsInHand[i] = hand.cardsInHand[i];
        hand.cardsInHand[i] = tempCardsInHandPointer;
    }

    Deck tempDeckPlayedWithPointer = *(this->deckPlayedWith);
    *(this->deckPlayedWith) = *(hand.deckPlayedWith);
    *(hand.deckPlayedWith) = tempDeckPlayedWithPointer;

    swap(this->associatedDeckPosition, hand.associatedDeckPosition);

    swap(this->handPosition, hand.handPosition);

    return *this;
}

//Plays a card in a certain position in the current hand, and returns it to the Deck afterwards.
Order* Hand::play(int cardIndex) {
    Order* order = this->cardsInHand[cardIndex]->play();
    //Removes the card from the hand, and sets the next empty spot in the hand to where the most recently played card originally was.
    this->cardsInHand[cardIndex] = nullptr;
    this->cardsInHand.erase(this->cardsInHand.begin() + cardIndex);
    handPosition = this->cardsInHand.size();
    return order;
}

//Adds a card to the current hand. Done in conjunction with the draw() method for the Deck class, which returns a drawn card (which is added to the current hand using this method).
void Hand::addToHand(Card* card) {
    //Adds the card to the first available position in the hand, indicated by handPosition.
    this->cardsInHand.push_back(card);
    //Sets handPosition to the next free position in the hand (the next one with a null pointer).
    handPosition = this->cardsInHand.size();
}

//Draws a card from the associated Deck, and adds it to the current hand.
void Hand::drawCard() {
    //Draws a card from the associated Deck, and adds it to the first available position in the hand, indicated by handPosition.
    this->cardsInHand.push_back(this->deckPlayedWith->draw());
    handPosition = this->cardsInHand.size();
}

//Displays all cards in the current hand.
void Hand::displayCardsInHand() {
    for (int i = 0; i < this->cardsInHand.size(); i++) {
        cout<<i<<": "<<this->cardsInHand[i]<<endl;
    }
}
