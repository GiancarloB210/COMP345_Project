#ifndef GAME_ENGINE_H
#define GAME_ENGINE_H

#include <string>
#include <map>

// All states in the game
enum class State {
    START,                  // Initial state before the game begins
    MAP_LOADED,             // Map has been loaded
    MAP_VALIDATED,          // Map has been validated
    PLAYERS_ADDED,          // Players have been added to the game
    ASSIGN_REINFORCEMENT,   // Reinforcements are being assigned
    ISSUE_ORDERS,           // Players are issuing orders
    EXECUTE_ORDERS,         // Orders are being executed
    WIN,                    // A player has won the game
    END                     // Game has ended
};

// All commands triggering state transitions
enum class Command {
    LOADMAP,        // Command to load a map
    VALIDATEMAP,    // Command to validate a map
    ADDPLAYER,      // Command to add players to the game
    ASSIGNCOUNTRIES,// Command to assign countries/reinforcements
    ISSUEORDER,     // Command to issue an order
    EXECORDER,      // Command to execute an order
    WIN,            // Command to declare a winner
    ENDGAME,        // Command to end the game
    INVALID         // Invalid command
};

// Engine class that controls state transitions
class GameEngine {
private:
    State currentState;  // Current state of the game
    std::map<State, std::map<std::string, State>> stateTransitions;  // Map of valid state transitions
    void setupTransitions();  // Function to setup valid state transitions

public:
    GameEngine();  // Constructor
    void handleCommand(const std::string& command);  // Function to handle user input commands
    void printState() const;  // Function to print the current state of the game
    bool isValidCommand(const std::string& command) const;  // Function to check if a command is valid
};

#endif
