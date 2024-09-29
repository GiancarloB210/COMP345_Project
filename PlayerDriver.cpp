#include "PlayerDriver.h"

#include "Player.h"

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
}
