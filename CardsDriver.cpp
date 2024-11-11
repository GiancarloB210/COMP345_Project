#include "CardsDriver.h"
#include "Cards.h"
#include "Player.h"

using namespace std;

//Methods for testing methods present in the card-related classes.

//Counts the number of cards in a hand's associated deck which can still be drawn.
//In essence: counts the cards "remaining" in the deck.
int countDrawableCards(Hand *hand) {
    int canBeDrawn = 0;
    for (int i = 0; i < 40;i++) {
        if (hand->deckPlayedWith->cards[i]->availableToDraw == true) {
            canBeDrawn++;
        }
    }
    return canBeDrawn;
}

//Tests several card-related methods.
void testCards() {
    Hand* hand = new Hand(new Deck());
    //Before drawing any cards, there should be 40 playable cards in the deck.
    cout<<"Cards in deck that can be played (before drawing any cards):"<<endl;
    int canBePlayedBefore = 0;
    for (int i = 0; i < 40;i++) {
        if (hand->deckPlayedWith->cards[i]->availableToDraw == true) {
            canBePlayedBefore++;
        }
    }
    cout<<canBePlayedBefore<<endl;
    //After drawing one card, there should be 39 drawable cards in the deck.
    //After drawing two cards, there should be 38 drawable cards in the deck.
    //After drawing three cards, there should be 37 drawable cards in the deck.
    hand->drawCard();
    cout<<"Cards in deck that can be played (after drawing 1 card):"<<endl;
    cout<<countDrawableCards(hand)<<endl;
    hand->drawCard();
    cout<<"Cards in deck that can be played (after drawing 2 cards):"<<endl;
    cout<<countDrawableCards(hand)<<endl;
    hand->drawCard();
    cout<<"Cards in deck that can be played (after drawing 3 cards):"<<endl;
    cout<<countDrawableCards(hand)<<endl;
    hand->drawCard();
    hand->drawCard();
    //Prints the contents of the deck in pseudo-detailed fashion, including information that
    //proves that 5 cards in the deck cannot be drawn (as expected).
    cout<<*hand->deckPlayedWith<<endl;
    cout<<"Drawable cards in deck (after all have been drawn): "<<countDrawableCards(hand)<<endl;
    //The number of cards in the hand should be 5, because 5 were drawn.
    cout<<"Hand size (before playing all cards):"<<endl;
    int sizeBefore = 0;
    for (int i = 0; i < 40;i++) {
        if (hand->cardsInHand[i] != nullptr) {
            sizeBefore++;
        }
    }
    cout<<sizeBefore<<endl;
    //Play all cards in the hand, and check that the functionality makes it so that only 2 cards
    //remain in the hand after playing 3.
    for (int i = 0;i < 40;i++) {
        //If a null pointer is encountered, all cards in the hand have been played.
        if (hand->cardsInHand[i] == nullptr) {
            break;
        }
        //Once there are two cards left in the hand, test to see if it holds true in the
        //context of the program.
        if (i == 3) {
            cout<<"Hand size (after playing 3 cards):"<<endl;
            //This value should be equal to 2.
            int sizeAfter3 = 0;
            for (int i2 = 0; i2 < 40;i2++) {
                if (hand->cardsInHand[i2] != nullptr) {
                    sizeAfter3++;
                }
            }
            cout<<sizeAfter3<<endl;
        }
        //Play the card in the current position in the hand.
        hand->play(i);
        //Rinse and repeat until all cards in the hand have been played.
    }
    //Since all cards in the hand have been played, it is expected that this value will equal 0.
    //There shouldn't be any more cards in the hand.
    cout<<"Hand size (after playing):"<<endl;
    int sizeAfter = 0;
    for (int i = 0; i < 40;i++) {
        if (hand->cardsInHand[i] != nullptr) {
            sizeAfter++;
        }
    }
    cout<<sizeAfter<<endl;
    //Prints the contents of the deck in pseudo-detailed fashion, including information that
    //proves that all cards in the deck can be drawn (as expected).
    cout<<*hand->deckPlayedWith<<endl;
    cout<<"Drawable cards in deck (after all have been played): "<<countDrawableCards(hand)<<endl;
}
