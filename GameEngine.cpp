#include "GameEngine.h"
#include <iostream>

// Default constructor 
GameEngine::GameEngine() : currentState(State::START) {
    setupTransitions();
}

// Copy constructor
GameEngine::GameEngine(const GameEngine& other) {
    // Deep copy of the current state and transitions
    this->currentState = other.currentState;
    this->stateTransitions = other.stateTransitions;
}

// Assignment operator
GameEngine& GameEngine::operator=(const GameEngine& other) {
    if (this != &other) {  // Self-assignment check
        this->currentState = other.currentState;
        this->stateTransitions = other.stateTransitions;
    }
    return *this;
}


// setup all valid state transitions based on commands and states
void GameEngine::setupTransitions() {
    // Transitions for the startup phase
    stateTransitions[State::START]["loadmap"] = State::MAP_LOADED;
    stateTransitions[State::MAP_LOADED]["validatemap"] = State::MAP_VALIDATED;
    stateTransitions[State::MAP_VALIDATED]["addplayer"] = State::PLAYERS_ADDED;

    // Transitions for the play phase
    stateTransitions[State::PLAYERS_ADDED]["assigncountries"] = State::ASSIGN_REINFORCEMENT;
    stateTransitions[State::ASSIGN_REINFORCEMENT]["issueorder"] = State::ISSUE_ORDERS;
    stateTransitions[State::ISSUE_ORDERS]["endissueorders"] = State::EXECUTE_ORDERS;
    stateTransitions[State::EXECUTE_ORDERS]["win"] = State::WIN;
    stateTransitions[State::EXECUTE_ORDERS]["endexecorders"] = State::ASSIGN_REINFORCEMENT;

    // End the game
    stateTransitions[State::WIN]["end"] = State::END;

    // Restart the game
    stateTransitions[State::WIN]["play"] = State::START;

    // Loop for issuing and executing orders
    stateTransitions[State::ISSUE_ORDERS]["issueorder"] = State::ISSUE_ORDERS;
    stateTransitions[State::EXECUTE_ORDERS]["execorder"] = State::EXECUTE_ORDERS;

    // Loop for loading maps and adding players
    stateTransitions[State::MAP_LOADED]["loadmap"] = State::MAP_LOADED;
    stateTransitions[State::PLAYERS_ADDED]["addplayer"] = State::PLAYERS_ADDED;
}

// handle user input commands and transition game states
void GameEngine::handleCommand(const std::string& command) {
    if (isValidCommand(command)) {
        currentState = stateTransitions[currentState][command];
        printState();
    } else {
        std::cout << "Invalid command: " << command << std::endl;
    }
}

// check if the command is valid for the current state
bool GameEngine::isValidCommand(const std::string& command) const {
    return stateTransitions.find(currentState) != stateTransitions.end() &&
           stateTransitions.at(currentState).find(command) != stateTransitions.at(currentState).end();
}

// print the current state of the game to the console
void GameEngine::printState() const {
    switch (currentState) {
        case State::START: {
            std::cout << "Current state: START" << std::endl;
            std::cout << "valid commands are: " << std::endl;
            for (const auto & stateTransition : stateTransitions) {
                if (stateTransition.first == currentState) {
                    for (const auto & i : stateTransition.second) {
                        std::cout << i.first << std::endl;
                    }
                }
            }
            std::cout << std::endl;
        }
            break;
        case State::MAP_LOADED: {
            std::cout << "Current state: MAP LOADED" << std::endl;
            std::cout << "valid commands are: " << std::endl;
            for (const auto & stateTransition : stateTransitions) {
                if (stateTransition.first == currentState) {
                    for (const auto & i : stateTransition.second) {
                        std::cout << i.first << std::endl;
                    }
                }
            }
            std::cout << std::endl;
        }
            break;
        case State::MAP_VALIDATED: {
            std::cout << "Current state: MAP VALIDATED" << std::endl;
            std::cout << "valid commands are: " << std::endl;
            for (const auto & stateTransition : stateTransitions) {
                if (stateTransition.first == currentState) {
                    for (const auto & i : stateTransition.second) {
                        std::cout << i.first << std::endl;
                    }
                }
            }
            std::cout << std::endl;
        }
            break;
        case State::PLAYERS_ADDED: {
            std::cout << "Current state: PLAYERS ADDED" << std::endl;
            std::cout << "valid commands are: " << std::endl;
            for (const auto & stateTransition : stateTransitions) {
                if (stateTransition.first == currentState) {
                    for (const auto & i : stateTransition.second) {
                        std::cout << i.first << std::endl;
                    }
                }
            }
            std::cout << std::endl;
        }
            break;
        case State::ASSIGN_REINFORCEMENT: {
            std::cout << "Current state: ASSIGN REINFORCEMENT" << std::endl;
            std::cout << "valid commands are: " << std::endl;
            for (const auto & stateTransition : stateTransitions) {
                if (stateTransition.first == currentState) {
                    for (const auto & i : stateTransition.second) {
                        std::cout << i.first << std::endl;
                    }
                }
            }
            std::cout << std::endl;
        }
            break;
        case State::ISSUE_ORDERS: {
            std::cout << "Current state: ISSUE ORDERS" << std::endl;
            std::cout << "valid commands are: " << std::endl;
            for (const auto & stateTransition : stateTransitions) {
                if (stateTransition.first == currentState) {
                    for (const auto & i : stateTransition.second) {
                        std::cout << i.first << std::endl;
                    }
                }
            }
            std::cout << std::endl;
        }
            break;
        case State::EXECUTE_ORDERS: {
            std::cout << "Current state: EXECUTE ORDERS" << std::endl;
            std::cout << "valid commands are: " << std::endl;
            for (const auto & stateTransition : stateTransitions) {
                if (stateTransition.first == currentState) {
                    for (const auto & i : stateTransition.second) {
                        std::cout << i.first << std::endl;
                    }
                }
            }
            std::cout << std::endl;
        }
            break;
        case State::WIN: {
            std::cout << "Current state: WIN" << std::endl;
            std::cout << "valid commands are: " << std::endl;
            for (const auto & stateTransition : stateTransitions) {
                if (stateTransition.first == currentState) {
                    for (const auto & i : stateTransition.second) {
                        std::cout << i.first << std::endl;
                    }
                }
            }
            std::cout << std::endl;
        }
            break;
        case State::END: {
            std::cout << "Current state: END" << std::endl;
            std::cout << "valid commands are: \nexit\n" << std::endl;
        }
            break;
    }
}

// Overloaded stream insertion operator 
std::ostream& operator<<(std::ostream& os, const GameEngine& engine) {
    os << "Current game state: ";
    switch (engine.currentState) {
        case State::START:
            os << "START";
            break;
        case State::MAP_LOADED:
            os << "MAP LOADED";
            break;
        case State::MAP_VALIDATED:
            os << "MAP VALIDATED";
            break;
        case State::PLAYERS_ADDED:
            os << "PLAYERS ADDED";
            break;
        case State::ASSIGN_REINFORCEMENT:
            os << "ASSIGN REINFORCEMENT";
            break;
        case State::ISSUE_ORDERS:
            os << "ISSUE ORDERS";
            break;
        case State::EXECUTE_ORDERS:
            os << "EXECUTE ORDERS";
            break;
        case State::WIN:
            os << "WIN";
            break;
        case State::END:
            os << "END";
            break;
    }
    return os;
}
