//
// Created by Dugua on 11/11/2024.
//

#include "CommandProcessingDriver.h"

#include "CommandProcessing.h"

void testCommandProcessor(GameEngine* gameEngine) {
    CommandProcessor* commandProcessor = gameEngine->getCommandProcessor();
    //FileCommandProcessorAdapter* commandProcessor = static_cast<FileCommandProcessorAdapter *>(gameEngine->getCommandProcessor());

    while (true) {

       std::cout << "enter a command: " << std::endl;
        Command* currentCommand = commandProcessor->getCommand();
        State currentState = gameEngine->getCurrentState();

        std::string commandString = currentCommand->getCommand();
        std::string delimeter = " ";
        std::string commandToken1 = commandString.substr(0, commandString.find(delimeter));
        std::string commandToken2 = commandString.substr(commandString.find(delimeter) + 1, commandString.length());

        std::cout << "command entered: " << commandToken1 << std::endl;

        if(commandProcessor->validate(commandToken1)) {
            if(commandToken1== "loadmap") {
                gameEngine->setCurrentState(State::MAP_LOADED);
                currentCommand->saveEffect("loaded map " + commandToken2);
                //gameEngine->loadMap();
            } else if(commandToken1== "validatemap") {
                gameEngine->setCurrentState(State::MAP_VALIDATED);
                currentCommand->saveEffect("validated map");
                //gameEngine->validateMap();
            } else if(commandToken1== "addplayer") {
                gameEngine->setCurrentState(State::PLAYERS_ADDED);
                currentCommand->saveEffect("added player " + commandToken2);
                //gameEngine->addPlayer();
            } else if(commandToken1== "gamestart") {
                gameEngine->setCurrentState(State::ASSIGN_REINFORCEMENT);
                currentCommand->saveEffect("game started");
                //gameEngine->startGame();
            } else if(commandToken1== "replay") {
                gameEngine->setCurrentState(State::START);
                currentCommand->saveEffect("replay");
                //gameEngine->startupPhase();
            } else if(commandToken1== "end") {
                gameEngine->setCurrentState(State::END);
                currentCommand->saveEffect("quit");
                exit(0);
            }
        }else {
            std::cout << "invalid command entered." << std::endl;
        }
    }
}
