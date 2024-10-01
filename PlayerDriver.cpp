#include "PlayerDriver.h"

#include "Player.h"
#include "Cards.h"

using namespace std;

PlayerDriver::PlayerDriver() {

}

void PlayerDriver::testPlayers() {
    Player player1;
    player1.issueOrder("Bomb");
    player1.issueOrder("Airlift");
    Player player2;
    player2.issueOrder("Diplomacy");
    player2.issueOrder("Blockade");
    player2.issueOrder("Reinforcement");
    cout<<"Player 1 has "<<size(player1.orderList->orders)<<" orders in their list of orders."<<endl;
    cout<<"Player 2 has "<<size(player2.orderList->orders)<<" orders in their list of orders."<<endl;
    player1.draw();
    player1.draw();
    player1.draw();
    player1.draw();
    player1.draw();
    for (Card* c : player1.playerHand->cardsInHand) {
        if (c != nullptr) {
            cout<<"Player 1 is playing a card."<<endl;
            player1.playCard(c->cardID);
        }
    }
    player2.draw();
    player2.draw();
    player2.draw();
    player2.draw();
    player2.draw();
    for (Card* c : player2.playerHand->cardsInHand) {
        if (c != nullptr) {
            cout<<"Player 2 is playing a card."<<endl;
            player2.playCard(c->cardID);
        }
    }
    cout<<"Player 1 has "<<size(player1.orderList->orders)<<" orders in their list of orders."<<endl;
    cout<<"Player 2 has "<<size(player2.orderList->orders)<<" orders in their list of orders."<<endl;

}
