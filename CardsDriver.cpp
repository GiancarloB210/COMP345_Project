#include "CardsDriver.h"

#include "Cards.h"
#include "Player.h"

using namespace std;

CardsDriver::CardsDriver() {

}

void CardsDriver::testCards() {
    Deck* deck = new Deck();
    Hand* hand = new Hand(deck);
    cout<<"Csrds in deck that can be played (before drawing any cards):"<<endl;
    int canBePlayedBefore = 0;
    for (int i = 0; i < 40;i++) {
        if (hand->deckPlayedWith->cards[i]->availableToDraw) {
            canBePlayedBefore++;
        }
    }
    cout<<canBePlayedBefore<<endl;
    // hand->addToHand(deck->draw());
    // hand->addToHand(deck->draw());
    // hand->addToHand(deck->draw());
    hand->drawCard();
    hand->drawCard();
    hand->drawCard();
    cout<<"Csrds in deck that can be played (after drawing 3 cards):"<<endl;
    int canBePlayedAfterDraw3 = 0;
    for (int i = 0; i < 40;i++) {
        if (hand->deckPlayedWith->cards[i]->availableToDraw) {
            canBePlayedAfterDraw3++;
        }
    }
    cout<<canBePlayedAfterDraw3<<endl;
    hand->addToHand(deck->draw());
    hand->addToHand(deck->draw());
    cout<<"Hand size (before playing all cards):"<<endl;
    int sizeBefore = 0;
    for (int i = 0; i < 40;i++) {
        if (hand->cardsInHand[i] != nullptr) {
            sizeBefore++;
        }
    }
    for (int i = 0;i < 40;i++) {
        if (hand->cardsInHand[i] == nullptr) {
            break;
        }
        if (i == 3) {
            cout<<"Hand size (after playing 3 cards):"<<endl;
            int sizeAfter3 = 0;
            for (int i2 = 0; i2 < 40;i2++) {
                if (hand->cardsInHand[i2] != nullptr) {
                    sizeAfter3++;
                }
            }
            cout<<sizeAfter3<<endl;
        }
        hand->play(i);
    }
    cout<<"Hand size (after playing):"<<endl;
    int sizeAfter = 0;
    for (int i = 0; i < size(hand->cardsInHand);i++) {
        if (hand->cardsInHand[i] != nullptr) {
            sizeAfter++;
        }
    }
    cout<<sizeAfter<<endl;
}

