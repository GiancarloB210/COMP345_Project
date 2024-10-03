#include "CardsDriver.h"

#include "Cards.h"
#include "Player.h"

using namespace std;

CardsDriver::CardsDriver() {

}

int CardsDriver::countDrawableCards(Hand *hand) {
    int canBeDrawn = 0;
    for (int i = 0; i < 40;i++) {
        if (hand->deckPlayedWith->cards[i]->availableToDraw == true) {
            canBeDrawn++;
        }
    }
    return canBeDrawn;
}

void CardsDriver::testCards() {
    Hand* hand = new Hand(new Deck());
    cout<<"Cards in deck that can be played (before drawing any cards):"<<endl;
    int canBePlayedBefore = 0;
    for (int i = 0; i < 40;i++) {
        if (hand->deckPlayedWith->cards[i]->availableToDraw == true) {
            canBePlayedBefore++;
        }
    }
    cout<<canBePlayedBefore<<endl;
    // hand->addToHand(deck->draw());
    // hand->addToHand(deck->draw());
    // hand->addToHand(deck->draw());
    hand->drawCard();
    cout<<"Cards in deck that can be played (after drawing 1 card):"<<endl;
    cout<<countDrawableCards(hand)<<endl;
    hand->drawCard();
    cout<<"Cards in deck that can be played (after drawing 2 cards):"<<endl;
    cout<<countDrawableCards(hand)<<endl;
    hand->drawCard();
    cout<<*hand->deckPlayedWith<<endl;
    cout<<"Cards in deck that can be played (after drawing 3 cards):"<<endl;
    cout<<countDrawableCards(hand)<<endl;
    hand->addToHand(hand->deckPlayedWith->draw());
    hand->addToHand(hand->deckPlayedWith->draw());
    cout<<"Hand size (before playing all cards):"<<endl;
    int sizeBefore = 0;
    for (int i = 0; i < 40;i++) {
        if (hand->cardsInHand[i] != nullptr) {
            sizeBefore++;
        }
    }
    cout<<sizeBefore<<endl;
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
    for (int i = 0; i < 40;i++) {
        if (hand->cardsInHand[i] != nullptr) {
            sizeAfter++;
        }
    }
    cout<<sizeAfter<<endl;
    hand->deckPlayedWith->clean();
    hand->clean();
}

// int main(int argc, char *argv[]) {
//     CardsDriver driver;
//     driver.testCards();
// }
