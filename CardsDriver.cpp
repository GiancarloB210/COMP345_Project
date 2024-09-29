#include "CardsDriver.h"

#include "Card.h"
#include "Player.h"

using namespace std;

CardsDriver::CardsDriver() {

}

void CardsDriver::testCards() {
    Deck* deck = new Deck();
    Hand* hand = new Hand(deck);
    hand->addToHand(deck->draw());
    hand->addToHand(deck->draw());
    hand->addToHand(deck->draw());
    hand->addToHand(deck->draw());
    hand->addToHand(deck->draw());
    for (int i = 0;i < size(hand->cardsInHand);i++) {
        if (hand->cardsInHand[i] == nullptr) {
            break;
        }
        hand->play(i);
    }
}

