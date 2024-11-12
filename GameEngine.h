#ifndef GAME_ENGINE_H
#define GAME_ENGINE_H

#include <string>
#include <map>
#include <iostream>
#include <vector>
#include <algorithm>

#include "Map.h"
#include "Player.h"
#include "Orders.h"

// All states in the game
enum class State {
    START,
    MAP_LOADED,
    MAP_VALIDATED,
    PLAYERS_ADDED,
    GAMESTART,
    ASSIGN_REINFORCEMENT,
    ISSUE_ORDERS,
    EXECUTE_ORDERS,
    WIN,
    END
};

// All commands that trigger state transitions
enum class Command {
    LOADMAP,
    VALIDATEMAP,
    ADDPLAYER,
    ASSIGNCOUNTRIES,
    ISSUEORDER,
    EXECORDER,
    WIN,
    ENDGAME,
    INVALID
};

// Game engine that controls state transitions
class GameEngine {
private:
    State currentState;  // Current state of the game
    std::map<State, std::map<std::string, State>> stateTransitions;  // Map of valid state transitions
    void setupTransitions();  // Function to setup valid state transitions
    std::string currentMapPath; // Currently loaded map path
    Map* currentMap; // Currently loaded map
    Deck* gameDeck; // Deck used for the game
    int numPlayers; // Number of players in the game
    std::vector<Player*> gamePlayers; // Vector of players in the game
    std::vector<int> playerOrder; // Turn order of players in the game

public:
    // Constructors
    GameEngine();  // Default constructor
    GameEngine(const GameEngine& other);  // Copy constructor
    GameEngine& operator=(const GameEngine& other);  // Assignment operator
    ~GameEngine();  // Destructor

    // Overloaded stream insertion operator
    friend std::ostream& operator<<(std::ostream& os, const GameEngine& engine);

    // Other functions
    void handleCommand(const std::string& command);
    void printState() const;
    bool isValidCommand(const std::string& command) const;
    void startupPhase();

    // Startup methods
    void loadMap();
    void validateMap();
    void setUpPlayers();
    void startGame();

    void distributeTerritories();
    void determinePlayerOrder();
    void allocateInitialArmies();
    void drawInitialCards();

    // Main game loop methods
    void mainGameLoop();             // Main game loop function
    void reinforcementPhase();        // Reinforcement phase function
    void issueOrdersPhase();          // Issuing orders phase function
    void executeOrdersPhase();        // Orders execution phase function
    bool isGameOver();                // Check if the game is over
};

#endif
