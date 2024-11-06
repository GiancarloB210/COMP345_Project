#include "GameEngine.h"
#include <fstream>
#include <iostream>
#include <filesystem>
#include <random>

namespace fs = std::__fs::filesystem;
using namespace std;

// Default constructor 
GameEngine::GameEngine() : currentState(State::START) {
    setupTransitions();
    this->currentMap = nullptr; //no map initialized as of yet.
    this->currentMapPath = ""; //no map initialized as of yet.
    this->gameDeck = new Deck();
    this->numPlayers = 1;
}

// Copy constructor
GameEngine::GameEngine(const GameEngine& other) {
    // Deep copy of the current state and transitions
    this->currentState = other.currentState;
    this->stateTransitions = other.stateTransitions;
    this->currentMap = new Map(*other.currentMap);
    this->currentMapPath = other.currentMapPath;
    this->gameDeck = new Deck(*other.gameDeck);
    this->numPlayers = other.numPlayers;
    //sizeof(other.gamePlayers)/sizeof(Player) will return the size of the other.gamePlayers array.
    for (int i = 0; i < sizeof(other.gamePlayers)/sizeof(other.gamePlayers[0]); i++) {
        Player* newPlayer = new Player(*other.gamePlayers[i]);
        this->gamePlayers[i] = newPlayer;
    }
    for (int i = 0;i < sizeof(this->playerOrder)/sizeof(this->playerOrder[0]);i++) {
        this->playerOrder[i] = other.playerOrder[i];
    }
}

// Assignment operator
GameEngine& GameEngine::operator=(const GameEngine& other) {
    if (this != &other) {  // Self-assignment check
        this->currentState = other.currentState;
        this->stateTransitions = other.stateTransitions;
        this->currentMap = new Map(*other.currentMap);
        this->currentMapPath = other.currentMapPath;
        this->gameDeck = new Deck(*other.gameDeck);
        this->numPlayers = other.numPlayers;
        //sizeof(other.gamePlayers)/sizeof(Player) will return the size of the other.gamePlayers array.
        for (int i = 0; i < sizeof(other.gamePlayers)/sizeof(other.gamePlayers[0]); i++) {
            Player* newPlayer = new Player(*other.gamePlayers[i]);
            this->gamePlayers[i] = newPlayer;
        }
        for (int i = 0;i < sizeof(this->playerOrder)/sizeof(this->playerOrder[0]);i++) {
            this->playerOrder[i] = other.playerOrder[i];
        }
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
    stateTransitions[State::PLAYERS_ADDED]["gamestart"] = State::ISSUE_ORDERS;
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
        if (command == "loadmap") {
            //The map files are located here.
            fs::directory_iterator mapFileDir("../MapFiles");
            int i = 0;
            string* maps = new string[4];
            for(auto& fileReference: mapFileDir) {
                std::cout << "[" << i << "] " << fileReference.path().string() << '\n';
                maps[i] = fileReference.path().string();
                i++;
            }
            int selectedMapIndex;
            cout << "Select the number of the map which you want to load." << endl;
            cin >> selectedMapIndex;
            while (selectedMapIndex < 0 || selectedMapIndex > maps->length()) {
                cout << "Invalid. Please enter a valid index." << endl;
                cin >> selectedMapIndex;
            }
            this->currentMapPath = maps[selectedMapIndex];
        }
        else if (command == "validatemap") {
            MapLoader mapLoader;
            this->currentMap = mapLoader.readFile(this->currentMapPath);
            if (!(currentMap->validate(currentMap))) {
                cout << "Invalid map loaded." << endl;
                currentState = State::START;
                printState();
            }
        }
        else if (command == "addplayer") {
            int numberOfPlayers;
            cout << "Enter the number of players (2-6) which you would like to add." << endl;
            cin >> numberOfPlayers;
            while (numberOfPlayers < 2 || numberOfPlayers > 6) {
                cout << "Invalid number. Please enter a number from 2 to 6." << endl;
                cin >> numberOfPlayers;
            }
            for (int i = 0;i < numberOfPlayers; i++) {
                string newPlayerName;
                cout << "Enter the name of player " << (i + 1) << endl;
                cin >> newPlayerName;
                Player* p = new Player(newPlayerName, new std::list<Territory*>, new Hand(gameDeck));
                cout << "Player created" << endl;
                this->gamePlayers.push_back(p);
                cout<<"Player vector size: "<<this->gamePlayers.size()<<endl;
            }
            cout<<"Player vector size: "<<(this->gamePlayers.size())<<endl;
        }
        else if (command == "gamestart") {

            //Fairly distribute the territories among players.
            std::list<Territory *> mapTerritories= this->currentMap->getTerritories();
            cout<<"mapTerritories: "<<mapTerritories.size()<<endl;
            cout<<"Player array size: "<<this->gamePlayers.size()<<endl;
            int territoriesPerPlayer = mapTerritories.size() / this->gamePlayers.size();
            int i, playerCount = 0;
            //for (int i = 0;i < this->gamePlayers.size();i++) {
            for (Territory* territory : mapTerritories) {
                this->gamePlayers[playerCount]->territories->push_back(territory);
                i++;
                if (i == territoriesPerPlayer) {
                    playerCount++;
                    i = 0;
                }
            }

            cout << "Territories distributed" << endl;

            //Randomly determines player order.
            std::vector<int> playerIDs;

            cout << "Player IDs added" << endl;

            //Code for shuffling an array and obtaining a true random generation using a time-oriented seed sourced from: https://cplusplus.com/reference/algorithm/shuffle/
            shuffle(std::begin(playerIDs), std::end(playerIDs), std::default_random_engine(std::chrono::system_clock::now().time_since_epoch().count()));
            this->playerOrder = playerIDs;

            cout << "Player IDs shuffled" << endl;

            //Allocates 50 army units to each player
            for (int j = 0;j < this->gamePlayers.size();j++) {
                for (int k = 0;k < 50;k++) {
                    this->gamePlayers[j]->armyUnits.push_back(new ArmyUnit());
                }
            }

            cout << "Army units allocated." << endl;

            //Each player draws 2 cards.
            for (int i = 0;i < this->gamePlayers.size();i++) {
                this->gamePlayers[i]->hand->drawCard();
                this->gamePlayers[i]->hand->drawCard();
            }

            cout << *this->gameDeck<<endl;

            cout << "Cards drawn." << endl;

            //Fix this ASAP.
            this->currentState = State::ASSIGN_REINFORCEMENT;
            printState();
        }
        else {
            std::cout << "Invalid command." << std::endl;
        }
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
