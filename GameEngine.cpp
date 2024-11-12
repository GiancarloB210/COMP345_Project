#include "GameEngine.h"
#include <fstream>
#include <iostream>
#include <filesystem>
#include <random>
#include <algorithm>

namespace fs = std::filesystem;
using namespace std;

// Default constructor 
GameEngine::GameEngine() : currentState(State::START) {
    setupTransitions();
    this->currentMap = nullptr; // No map initialized as of yet.
    this->currentMapPath = ""; // No map initialized as of yet.
    this->gameDeck = new Deck();
    this->numPlayers = 1;
}

// Copy constructor
GameEngine::GameEngine(const GameEngine& other) {
    // Copy all fields with deep copies for complex members
    this->currentState = other.currentState;
    this->stateTransitions = other.stateTransitions;
    this->currentMap = new Map(*other.currentMap);
    this->currentMapPath = other.currentMapPath;
    this->gameDeck = new Deck(*other.gameDeck);
    this->numPlayers = other.numPlayers;

    // Deep copy for players
    for (int i = 0; i < other.gamePlayers.size(); i++) {
        Player* newPlayer = new Player(*other.gamePlayers[i]);
        this->gamePlayers.push_back(newPlayer);
    }

    // Copy player order
    this->playerOrder = other.playerOrder;
}

// Assignment operator
GameEngine& GameEngine::operator=(const GameEngine& other) {
    if (this != &other) {  // Self-assignment check
        // Perform deep copy as in copy constructor
        this->currentState = other.currentState;
        this->stateTransitions = other.stateTransitions;
        this->currentMap = new Map(*other.currentMap);
        this->currentMapPath = other.currentMapPath;
        this->gameDeck = new Deck(*other.gameDeck);
        this->numPlayers = other.numPlayers;

        // Deep copy for players
        this->gamePlayers.clear();
        for (int i = 0; i < other.gamePlayers.size(); i++) {
            Player* newPlayer = new Player(*other.gamePlayers[i]);
            this->gamePlayers.push_back(newPlayer);
        }

        // Copy player order
        this->playerOrder = other.playerOrder;
    }
    return *this;
}

// Destructor
GameEngine::~GameEngine() {
    delete this->gameDeck;
    this->gameDeck = nullptr;
    for (Player* player : this->gamePlayers) {
        delete player;
    }
    this->gamePlayers.clear();
}

// Setup state transitions
void GameEngine::setupTransitions() {
    // Transitions for the startup phase and game phases
    stateTransitions[State::START]["loadmap"] = State::MAP_LOADED;
    stateTransitions[State::MAP_LOADED]["validatemap"] = State::MAP_VALIDATED;
    stateTransitions[State::MAP_VALIDATED]["addplayer"] = State::PLAYERS_ADDED;
    stateTransitions[State::PLAYERS_ADDED]["assigncountries"] = State::ASSIGN_REINFORCEMENT;
    stateTransitions[State::ASSIGN_REINFORCEMENT]["issueorder"] = State::ISSUE_ORDERS;
    stateTransitions[State::ISSUE_ORDERS]["endissueorders"] = State::EXECUTE_ORDERS;
    stateTransitions[State::EXECUTE_ORDERS]["win"] = State::WIN;
    stateTransitions[State::EXECUTE_ORDERS]["endexecorders"] = State::ASSIGN_REINFORCEMENT;
    stateTransitions[State::WIN]["end"] = State::END;
    stateTransitions[State::WIN]["play"] = State::START;
    stateTransitions[State::ISSUE_ORDERS]["issueorder"] = State::ISSUE_ORDERS;
    stateTransitions[State::EXECUTE_ORDERS]["execorder"] = State::EXECUTE_ORDERS;
    stateTransitions[State::MAP_LOADED]["loadmap"] = State::MAP_LOADED;
    stateTransitions[State::PLAYERS_ADDED]["addplayer"] = State::PLAYERS_ADDED;
}

// Command handling function
void GameEngine::handleCommand(const std::string& command) {
    if (isValidCommand(command)) {
        currentState = stateTransitions[currentState][command];
        printState();
    } else {
        std::cout << "Invalid command: " << command << std::endl;
    }
}

// Startup phase (existing functionality remains unchanged)

// check if the command is valid for the current state
bool GameEngine::isValidCommand(const std::string& command) const {
    return stateTransitions.find(currentState) != stateTransitions.end() &&
           stateTransitions.at(currentState).find(command) != stateTransitions.at(currentState).end();
}

// print the current state of the game to the console
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
        case State::GAMESTART:
            std::cout << "Current state: GAME START" << std::endl;
            break;
        case State::END:
            std::cout << "Current state: END" << std::endl;
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

// Main Game Loop Method
void GameEngine::mainGameLoop() {
    while (!isGameOver()) {
        reinforcementPhase();
        issueOrdersPhase();
        executeOrdersPhase();
    }
    std::cout << "Game over! The winner is " << gamePlayers[0]->getName() << "!" << std::endl;
}

// Reinforcement Phase Method
void GameEngine::reinforcementPhase() {
    for (Player* player : gamePlayers) {
        int reinforcementUnits = std::max(3, static_cast<int>(player->getTerritories().size() / 3));
        player->addToReinforcementPool(reinforcementUnits);
        std::cout << player->getName() << " receives " << reinforcementUnits << " army units." << std::endl;
    }
}

// Issue Orders Phase Method
void GameEngine::issueOrdersPhase() {
    bool ordersPending;
    do {
        ordersPending = false;
        for (Player* player : gamePlayers) {
            if (player->hasMoreOrders()) {
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
    return activePlayers == 1;
}
