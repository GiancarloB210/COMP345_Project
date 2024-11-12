#ifndef GAME_ENGINE_H
#define GAME_ENGINE_H

#include <string>
#include <map>
#include <iostream>

#include "Map.h"

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
    string currentMapPath; //Currently loaded map path.
    Map* currentMap; //Currently loaded map.
    Deck* gameDeck; //Deck which will be used for the game.
    int numPlayers; //Number of players who will be playing the game.
    std::vector<Player*> gamePlayers; //Vector of players who will be playing the game.
    std::vector<int> playerOrder; //Turn order of players who will be playing the game.

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

    //Startup methods
    void loadMap();
    void validateMap();
    void setUpPlayers();
    void startGame();
};

#endif
