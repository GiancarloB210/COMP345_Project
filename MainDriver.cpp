//
// Created by Dugua on 10/3/2024.
//

#include "MainDriver.h"

#include <OrdersDriver.h>
#include <string.h>

#include "CardsDriver.h"
#include "MapDriver.h"
#include "PlayerDriver.h"
#include "GameEngineDriver.h"
#include "GameEngine.h"
#include "Map.h"
#include "Player.h"
#include "Cards.h"
#include "CommandProcessing.h"
#include "CommandProcessingDriver.h"
#include "Orders.h"

int main(int argc, char* argv[]) {
    std::vector<std::string> args(argv, argv+argc);
    GameEngine* gameEngine;

    if (args[1] == "-console") {
        cout << "proceeding with console commands." << endl;
        gameEngine = new GameEngine(new CommandProcessor(gameEngine));
    }
    else if (args[1] == "-file") {
        cout << "proceeding with file commands." << endl;
        std::string filename = args[2];
        gameEngine = new GameEngine(new FileCommandProcessorAdapter(filename, gameEngine));
    }
    else {
        cout << "no command line arguments detected, proceeding with console commands" << endl;
        gameEngine = new GameEngine(new CommandProcessor(gameEngine));
    }

    gameEngine->startupPhase();

    //testGameStates();
    //testStartupPhase();
    //testCommandProcessor(gameEngine);
    //testMainGameLoop();
    //testOrdersExecution();

    return 0;
}
