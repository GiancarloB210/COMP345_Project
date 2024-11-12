#include "GameEngine.h"
#include <fstream>
#include <iostream>
#include <filesystem>
#include <random>

namespace fs = std::filesystem;
using namespace std;

// Default constructor 
GameEngine::GameEngine() : currentState(State::START) {
    setupTransitions();
    this->currentMap = nullptr;
    this->currentMapPath = "";
    this->gameDeck = new Deck();
    this->numPlayers = 1;
}

GameEngine::GameEngine(CommandProcessor* commandProcessor) : currentState(State::START) {
    setupTransitions();
    this->commandProcessor = commandProcessor;
    this->currentMap = nullptr; //no map initialized as of yet.
    this->currentMapPath = ""; //no map initialized as of yet.
    this->gameDeck = new Deck();
    this->numPlayers = 1;
}

// Copy constructor
GameEngine::GameEngine(const GameEngine& other) {
    this->currentState = other.currentState;
    this->commandProcessor = other.commandProcessor;
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
    if (this != &other) {
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

// Accessor
CommandProcessor* GameEngine::getCommandProcessor() {
    return this->commandProcessor;
}

// setup all valid state transitions based on commands and states
void GameEngine::setupTransitions() {
    stateTransitions[State::START]["loadmap"] = State::MAP_LOADED;
    stateTransitions[State::MAP_LOADED]["validatemap"] = State::MAP_VALIDATED;
    stateTransitions[State::MAP_VALIDATED]["addplayer"] = State::PLAYERS_ADDED;
    stateTransitions[State::PLAYERS_ADDED]["gamestart"] = State::ASSIGN_REINFORCEMENT;
    stateTransitions[State::ASSIGN_REINFORCEMENT]["issueorder"] = State::ISSUE_ORDERS;
    stateTransitions[State::ISSUE_ORDERS]["endissueorders"] = State::EXECUTE_ORDERS;
    stateTransitions[State::EXECUTE_ORDERS]["win"] = State::WIN;
    stateTransitions[State::EXECUTE_ORDERS]["endexecorders"] = State::ASSIGN_REINFORCEMENT;
    stateTransitions[State::WIN]["end"] = State::END;
    stateTransitions[State::WIN]["replay"] = State::START;
    stateTransitions[State::ISSUE_ORDERS]["issueorder"] = State::ISSUE_ORDERS;
    stateTransitions[State::EXECUTE_ORDERS]["execorder"] = State::EXECUTE_ORDERS;
    stateTransitions[State::MAP_LOADED]["loadmap"] = State::MAP_LOADED;
}


// Main Game Loop Method
void GameEngine::mainGameLoop() {
    while (!isGameOver()) {
        reinforcementPhase();   // Assign reinforcements automatically
        issueOrdersPhase();     // Players issue orders in a round-robin fashion
        executeOrdersPhase();   // Execute orders in a round-robin fashion
    }
    std::cout << "Game over! The winner is " << gamePlayers[0]->getName() << "!" << std::endl;
}

// Reinforcement Phase Method
void GameEngine::reinforcementPhase() {
    for (Player* player : gamePlayers) {
        // Calculate the number of reinforcements based on territories owned
        int baseReinforcements = std::max(3, static_cast<int>(player->getTerritories().size() / 3));
        int continentBonus = 0;

        // Calculate continent bonus
        for (const auto& continent : currentMap->getContinents()) {
            bool ownsAll = true;
            for (const auto& territory : continent->getTerritories()) {
                if (territory->getPlayer() != player) {
                    ownsAll = false;
                    break;
                }
            }
            if (ownsAll) {
                continentBonus += continent->getScore();
            }
        }

        // Total reinforcements
        int totalReinforcements = baseReinforcements + continentBonus;
        player->addToReinforcementPool(totalReinforcements);

        // Output message about reinforcements
        std::cout << player->getName() << " receives " << totalReinforcements << " army units." << std::endl;
    }

    // After reinforcements, explicitly transition to the next phase (Issue Orders)
    currentState = State::ISSUE_ORDERS;  // Transition to the issue orders phase
    std::cout << "Transitioning to the Issue Orders Phase." << std::endl;
}



// Issue Orders Phase Method
void GameEngine::issueOrdersPhase() {
    bool ordersPending;
    do {
        ordersPending = false;
        for (Player* player : gamePlayers) {
            if (player->reinforcementPool > 0 || player->hasMoreOrders()) {
                player->issueOrder();
                ordersPending = true;
            }
        }
    } while (ordersPending);
}

// Execute Orders Phase Method
void GameEngine::executeOrdersPhase() {
    bool ordersLeft;
    do {
        ordersLeft = false;
        for (Player* player : gamePlayers) {
            if (player->hasMoreOrders()) {
                player->getNextOrder()->execute();
                ordersLeft = true;
            }
        }
    } while (ordersLeft);
}

// Check if the game is over
bool GameEngine::isGameOver() {
    int activePlayers = 0;
    for (Player* player : gamePlayers) {
        if (player->ownsTerritory()) {
            activePlayers++;
        }
    }
    return activePlayers <= 1;
}

void GameEngine::handleCommand(const std::string& command) {
    if (isValidCommand(command)) {
        if (this->currentState == State::START && command == "loadmap") {
            loadMap();
        }
        else if (this->currentState == State::MAP_LOADED && command == "validatemap") {
            validateMap();
        }
        else if (this->currentState == State::MAP_VALIDATED && command == "addplayer") {
            setUpPlayers();  // Only add players, don't set the state here since setUpPlayers handles it
        }
        else if (this->currentState == State::PLAYERS_ADDED && command == "gamestart") {
            startGame();  // Transition from PLAYERS_ADDED to ASSIGN_REINFORCEMENT
            mainGameLoop();  // Begin the main game loop
        }
        // handle transitions for ISSUE_ORDERS phase
        else if (this->currentState == State::ISSUE_ORDERS && command == "issueorder") {
            issueOrdersPhase();  // Start issuing orders
        }

        // Update the state after the command execution if there was no internal state change
        if (stateTransitions[currentState].find(command) != stateTransitions[currentState].end()) {
            currentState = stateTransitions[currentState][command];
        }
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
    // Sets the number of players to be added to the game.
    int numberOfPlayers;
    bool validPlayerNumberCheck = false;

    // Prevents the user from inputting a non-numerical input or invalid number of players.
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

    // Every player only needs a name to be inputted.
    for (int i = 0; i < numberOfPlayers; i++) {
        string newPlayerName;
        cout << "Enter the name of player " << (i + 1) << endl;
        cin >> newPlayerName;
        Player* p = new Player(newPlayerName, new std::list<Territory*>, new Hand(gameDeck));

        // Add the player to the list of game players.
        this->gamePlayers.push_back(p);
    }

    // Transition state to PLAYERS_ADDED once players are added
    currentState = State::PLAYERS_ADDED;
    cout << "Players added. Now, issue the gamestart command to start the game!" << endl;
}



void GameEngine::distributeTerritories() {
    std::list<Territory *> mapTerritories = this->currentMap->getTerritories();
    int territoriesPerPlayer = mapTerritories.size() / this->gamePlayers.size();
    int territoryCounter = 0, playerCount = 0;

    for (Territory* territory : mapTerritories) {
        territory->setPlayer(this->gamePlayers[playerCount]);
        this->gamePlayers[playerCount]->territories->push_back(territory);
        territoryCounter++;

        if (territoryCounter == territoriesPerPlayer) {
            playerCount++;
            territoryCounter = 0;
        }
    }
    cout << "Territories distributed to players." << endl;
}

void GameEngine::determinePlayerOrder() {
    // Randomly shuffle the player order
    std::vector<int> playerIDs(this->gamePlayers.size());
    for (int i = 0; i < this->gamePlayers.size(); i++) {
        playerIDs[i] = i;
    }

    shuffle(playerIDs.begin(), playerIDs.end(), std::default_random_engine(std::chrono::system_clock::now().time_since_epoch().count()));

    // Store the shuffled player order
    for (int id : playerIDs) {
        this->playerOrder.push_back(id);
    }
    cout << "Player order randomized." << endl;
}

void GameEngine::allocateInitialArmies() {
    for (int j = 0; j < this->gamePlayers.size(); j++) {
        for (int k = 0; k < 50; k++) {
            this->gamePlayers[j]->armyUnits.push_back(new ArmyUnit(this->gamePlayers[j]));
        }
    }
    cout << "Army units allocated to players." << endl;
}

void GameEngine::drawInitialCards() {
    for (int i = 0; i < this->gamePlayers.size(); i++) {
        this->gamePlayers[i]->hand->drawCard();
        this->gamePlayers[i]->hand->drawCard();
    }
    cout << "Initial cards drawn for players." << endl;
}



void GameEngine::startGame() {
    cout << "The game has been started." << endl;

    // Fairly distribute all the territories to the players
    distributeTerritories();

    // Randomly determine the order of play
    determinePlayerOrder();

    // Give 50 initial army units to each player
    allocateInitialArmies();

    // Let each player draw 2 initial cards from the deck
    drawInitialCards();

    // Switch the game to the play phase
    currentState = State::ASSIGN_REINFORCEMENT; // Ready for reinforcement phase
    cout << "Game started. It's now time for the reinforcement phase!" << endl;
}


void GameEngine::startupPhase() {
    string inputCommand;
    while (this->currentState != State::ASSIGN_REINFORCEMENT) {
        this->printState();
        std::cout << "Enter command (type 'exit' to stop): ";

        //std::cin >> inputCommand;
        //std::cout << std::endl;
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

std::string GameEngine::stringToLog() {  

}