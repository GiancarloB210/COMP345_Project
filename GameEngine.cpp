#include "GameEngine.h"
#include <iostream>

// Initializing the game engine & Setting up state transitions
GameEngine::GameEngine() : currentState(State::START) {
    setupTransitions();  // Setup the valid state transitions
}

// Defining the valid state transitions
void GameEngine::setupTransitions() {
    // startup phase
    stateTransitions[State::START]["loadmap"] = State::MAP_LOADED;
    stateTransitions[State::MAP_LOADED]["validatemap"] = State::MAP_VALIDATED;
    stateTransitions[State::MAP_VALIDATED]["addplayer"] = State::PLAYERS_ADDED;

    // play phase
    stateTransitions[State::PLAYERS_ADDED]["assigncountries"] = State::ASSIGN_REINFORCEMENT;
    stateTransitions[State::ASSIGN_REINFORCEMENT]["issueorder"] = State::ISSUE_ORDERS;
    stateTransitions[State::ISSUE_ORDERS]["execorder"] = State::EXECUTE_ORDERS;
    stateTransitions[State::EXECUTE_ORDERS]["win"] = State::WIN;

    // ending the game
    stateTransitions[State::WIN]["end"] = State::END;

    // issuing and executing orders
    stateTransitions[State::ISSUE_ORDERS]["issueorder"] = State::ISSUE_ORDERS;
    stateTransitions[State::EXECUTE_ORDERS]["execorder"] = State::EXECUTE_ORDERS;
}

// Handling Input command and transition the state
void GameEngine::handleCommand(const std::string& command) {
    // Check if the command is valid for the current state
    if (isValidCommand(command)) {
        // Transition to the next state based on the command
        currentState = stateTransitions[currentState][command];
        printState();  // Output the new current state
    } else {
        // Invalid command, display an error message
        std::cout << "Invalid command: " << command << std::endl;
    }
}

// Check if the entered command is valid for the current state
bool GameEngine::isValidCommand(const std::string& command) const {
    return stateTransitions.find(currentState) != stateTransitions.end() &&
           stateTransitions.at(currentState).find(command) != stateTransitions.at(currentState).end();
}

// Print current state of the game to console
void GameEngine::printState() const {
    switch (currentState) {
        case State::START:
            std::cout << "Current state: START" << std::endl;
            break;
        case State::MAP_LOADED:
            std::cout << "Current state: MAP LOADED" << std::endl;
            break;
        case State::MAP_VALIDATED:
            std::cout << "Current state: MAP VALIDATED" << std::endl;
            break;
        case State::PLAYERS_ADDED:
            std::cout << "Current state: PLAYERS ADDED" << std::endl;
            break;
        case State::ASSIGN_REINFORCEMENT:
            std::cout << "Current state: ASSIGN REINFORCEMENT" << std::endl;
            break;
        case State::ISSUE_ORDERS:
            std::cout << "Current state: ISSUE ORDERS" << std::endl;
            break;
        case State::EXECUTE_ORDERS:
            std::cout << "Current state: EXECUTE ORDERS" << std::endl;
            break;
        case State::WIN:
            std::cout << "Current state: WIN" << std::endl;
            break;
        case State::END:
            std::cout << "Current state: END" << std::endl;
            break;
    }
}
