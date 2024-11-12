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
    for (int i = 0; i < other.gamePlayers.size(); i++) {
        Player* newPlayer = new Player(*other.gamePlayers[i]);
        this->gamePlayers.push_back(newPlayer);
    }
    for (int i = 0;i < this->playerOrder.size();i++) {
        this->playerOrder.push_back(other.playerOrder[i]);
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
        for (int i = 0; i < other.gamePlayers.size(); i++) {
            Player* newPlayer = new Player(*other.gamePlayers[i]);
            this->gamePlayers.push_back(newPlayer);
        }
        for (int i = 0;i < this->playerOrder.size();i++) {
            this->playerOrder.push_back(other.playerOrder[i]);
        }
    }
    return *this;
}

GameEngine::~GameEngine() {
    delete this->gameDeck;
    this->gameDeck = NULL;
    int size = this->gamePlayers.size();
    for (int i = 0; i < size; i++) {
        delete this->gamePlayers[i];
        this->gamePlayers[i] = NULL;
    }
}

// setup all valid state transitions based on commands and states
void GameEngine::setupTransitions() {
    // Transitions for the startup phase
    stateTransitions[State::START]["loadmap"] = State::MAP_LOADED; //Required for Part 2. A shortcut has been added for selecting the map from a list of files from a directory where all of the map files are
    stateTransitions[State::MAP_LOADED]["validatemap"] = State::MAP_VALIDATED; //Required for Part 2.
    stateTransitions[State::MAP_VALIDATED]["addplayer"] = State::PLAYERS_ADDED; //Required for Part 2. A shortcut has been taken for allowing all of the players to be added at once.

    // Transitions for the play phase
    stateTransitions[State::PLAYERS_ADDED]["assigncountries"] = State::ASSIGN_REINFORCEMENT;
    stateTransitions[State::PLAYERS_ADDED]["gamestart"] = State::ASSIGN_REINFORCEMENT; //Required for Part 2.
    stateTransitions[State::ASSIGN_REINFORCEMENT]["issueorder"] = State::ISSUE_ORDERS;
    stateTransitions[State::ISSUE_ORDERS]["endissueorders"] = State::EXECUTE_ORDERS;
    stateTransitions[State::EXECUTE_ORDERS]["win"] = State::WIN;
    stateTransitions[State::EXECUTE_ORDERS]["endexecorders"] = State::ASSIGN_REINFORCEMENT;

    // End the game
    stateTransitions[State::WIN]["end"] = State::END;

    // Restart the game
    stateTransitions[State::WIN]["replay"] = State::START;

    // Loop for issuing and executing orders
    stateTransitions[State::ISSUE_ORDERS]["issueorder"] = State::ISSUE_ORDERS;
    stateTransitions[State::EXECUTE_ORDERS]["execorder"] = State::EXECUTE_ORDERS;

    // Loop for loading maps and adding players
    stateTransitions[State::MAP_LOADED]["loadmap"] = State::MAP_LOADED;
    //stateTransitions[State::PLAYERS_ADDED]["addplayer"] = State::PLAYERS_ADDED; //Not needed due to all players being added at once in earlier line.
}

// handle user input commands and transition game states
void GameEngine::handleCommand(const std::string& command) {
    if (isValidCommand(command)) {
        if (this->currentState == State::START && command == "loadmap") {
            loadMap();
        }
        if (this->currentState == State::MAP_LOADED && command == "loadmap") {
            loadMap();
        }
        if (this->currentState == State::MAP_LOADED && command == "validatemap") {
            validateMap();
        }
        if (this->currentState == State::MAP_VALIDATED && command == "addplayer") {
            setUpPlayers();
        }
        if (this->currentState == State::PLAYERS_ADDED && command == "startGame") {
            startGame();
        }
        currentState = stateTransitions[currentState][command];
        printState();
    } else {
        std::cout << "Invalid command: " << command << std::endl;
    }
}

void GameEngine::loadMap() {
    //The map files are located here.
    fs::directory_iterator mapFileDir("../MapFiles");
    int mapFileIndex = 0;

    //There are 4 maps in the above directory.
    std::vector<string> mapFilePaths;

    //Prints the relative (not absolute) path of every map file for selection.
    for(auto& fileReference: mapFileDir) {
        std::cout << "[" << mapFileIndex << "] " << fileReference.path().string() << '\n';
        mapFilePaths.push_back(fileReference.path().string());
        mapFileIndex++;
    }

    //The user will input the number corresponding to the map file path associated
    //with the map file they wish to select.
    int selectedMapIndex;
    bool validIndexInputCheck = false;
    bool validMapCheck = false;

    //Prevents the user from inputting a non-numerical input or invalid index.
    while (!validIndexInputCheck || !validMapCheck) {
        cout << "Select the number of the map which you want to load." << endl;
        cin >> selectedMapIndex;
        if (cin.fail() || (selectedMapIndex < 0 || selectedMapIndex >= mapFilePaths.size())) {
            cin.clear();
            cin.ignore();
            validIndexInputCheck = false;
            cout << "Invalid. Please enter a valid index." << endl;
        }
        else {
            validIndexInputCheck = true;

            //The file at this path will be validated in the sequentially next state.
            this->currentMapPath = mapFilePaths[selectedMapIndex];

            //Validate that the map is valid. If it's invalid, return to the start state.
            //If it's valid, continue as usual.
            MapLoader mapLoader;

            //Reads the map from the map file path selected earlier.
            this->currentMap = mapLoader.readFile(this->currentMapPath);

            //Return to the initial state if the map is invalid.
            if (!(currentMap->validate(currentMap))) {
                cout << "Invalid map loaded. Please try again." << endl;
            } else {
                validMapCheck = true;
            }
        }
    }

    //The file at this path will be validated in the sequentially next state.
    this->currentMapPath = mapFilePaths[selectedMapIndex];
}

void GameEngine::validateMap() {
    //Validate that the map is valid. If it's invalid, return to the start state.
    //If it's valid, continue as usual.
    MapLoader mapLoader;

    //Reads the map from the map file path selected earlier.
    this->currentMap = mapLoader.readFile(this->currentMapPath);

    //Return to the initial state if the map is invalid.
    if (!(currentMap->validate(currentMap))) {
        cout << "Invalid map loaded." << endl;
        currentState = State::START;
    }
}

void GameEngine::setUpPlayers() {
    //Sets the number of players to be added to the game.
    int numberOfPlayers;
    bool validPlayerNumberCheck = false;

    //Prevents the user from inputting a non-numerical input or invalid number of players.
    while (!validPlayerNumberCheck) {
        cout << "Enter the number of players (2-6) which you would like to add." << endl;
        cin >> numberOfPlayers;
        if (cin.fail() || (numberOfPlayers < 2 || numberOfPlayers > 6)) {
            cin.clear();
            cin.ignore();
            cout << "Invalid. Please enter a valid number of players (from 2 to 6)." << endl;
        } else {
            validPlayerNumberCheck = true;
        }
    }

    //Every player only needs a name to be inputted, since their list of territories
    //is empty by default and their hands all pertain to the game deck.
    for (int i = 0;i < numberOfPlayers; i++) {
        string newPlayerName;
        cout << "Enter the name of player " << (i + 1) << endl;
        cin >> newPlayerName;
        Player* p = new Player(newPlayerName, new std::list<Territory*>, new Hand(gameDeck));

        //Add the player to the list of game players.
        this->gamePlayers.push_back(p);
    }

    //Fairly distribute the territories among players.
    std::list<Territory *> mapTerritories= this->currentMap->getTerritories();
    cout<<"mapTerritories: "<<mapTerritories.size()<<endl;
    cout<<"Number of players: "<<this->gamePlayers.size()<<endl;
    //Establishes how many territories each player should receive
    //such that the distribution is fair.
    int territoriesPerPlayer = mapTerritories.size() / this->gamePlayers.size();
    cout<<"Territories per player: " << territoriesPerPlayer<<endl;
    int territoryCounter = 0, playerCount = 0;
    for (Territory* territory : mapTerritories) {
        //Makes sure that every player receives the number of territories that each player should have
        //in accordance with fair distribution. Also, set a player's territory's associated player to that player.
        territory->setPlayer(this->gamePlayers[playerCount]);
        this->gamePlayers[playerCount]->territories->push_back(territory);
        territoryCounter++;
        //If a player has been assigned their maximum number of territories,
        //move on to the next player (the next territory will be assigned to them).
        if (territoryCounter == territoriesPerPlayer) {
            playerCount++;
            //Checks to see if enough territories can be allocated.
            //If not, move on to next step (shuffling player IDs for turn order determination).
            if ((mapTerritories.size() - (playerCount * territoryCounter)) < territoriesPerPlayer) {
                break;
            }
            territoryCounter = 0;
        }
    }
    cout << "Territories distributed" << endl;

    //Randomly determines player order.
    std::vector<int> playerIDs;

    cout << "Player IDs added" << endl;

    //Code for shuffling an array and obtaining a true random generation using a time-oriented seed sourced from: https://cplusplus.com/reference/algorithm/shuffle/
    shuffle(std::begin(this->playerOrder), std::end(this->playerOrder), std::default_random_engine(std::chrono::system_clock::now().time_since_epoch().count()));

    cout << "Player IDs shuffled" << endl;

    //Allocates 50 army units to each player
    for (int j = 0;j < this->gamePlayers.size();j++) {
        for (int k = 0;k < 50;k++) {
            this->gamePlayers[j]->armyUnits.push_back(new ArmyUnit(this->gamePlayers[j]));
        }
    }

    cout << "Army units allocated." << endl;

    //Each player draws 2 cards.
    for (int i = 0;i < this->gamePlayers.size();i++) {
        this->gamePlayers[i]->hand->drawCard();
        this->gamePlayers[i]->hand->drawCard();
    }

    cout << "Cards drawn." << endl;
}

void GameEngine::startGame() {
    cout<<"The game has been started"<<endl;
}

void GameEngine::startupPhase() {
    string inputCommand;
    while (this->currentState != State::ASSIGN_REINFORCEMENT) {
        if (this->currentState == State::START) {
            this->printState();
        }
        std::cout << "Enter command (type 'exit' to stop): ";
        std::cin >> inputCommand;
        std::cout << std::endl;
        // Exit the loop if the user types 'exit'
        if (inputCommand == "exit") {
            break;
        }
        this->handleCommand(inputCommand);
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
        case State::GAMESTART: {
            std::cout << "Current state: GAMESTART" << std::endl;
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
        case State::GAMESTART:
            os << "GAME START";
            break;
        default:
            os << "OTHER STATE";
            break;
    }
    return os;
}
