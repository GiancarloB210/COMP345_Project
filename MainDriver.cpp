//
// Created by Dugua on 10/3/2024.
//

#include "MainDriver.h"
#include "CardsDriver.h"
#include "MapDriver.h"
#include "PlayerDriver.h"
#include "Map.h"
#include "Player.h"
#include "Cards.h"
#include "Orders.h"

int main() {

    testLoadMaps();
    testPlayers();
    CardsDriver* cardsDriver = new CardsDriver();
    cardsDriver->testCards();

    return 0;
}
