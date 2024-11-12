#include "GameEngine.h"
#include <iostream>
#include <string>

// Test the game engine state transitions based on user commands
void testGameStates() {
    GameEngine engine; 
    std::string command;

    engine.printState();
    // Continuously prompt the user for commands
    while (true) {
        std::cout << "Enter command (type 'exit' to stop): ";
        std::cin >> command;
        std::cout << std::endl;

        // Exit the loop if the user types 'exit'
        if (command == "exit") {
            break;
        }

        engine.handleCommand(command);
    }

    // Print the final state of the game engine
    std::cout << engine << std::endl;
}

// Test the game engine startup phase
void testStartupPhase() {
    GameEngine engine;
    engine.startupPhase();
    // Print the final state of the game engine
    std::cout << engine << std::endl;
}

// Test the main game loop
void testMainGameLoop() {
    GameEngine engine;
    engine.startupPhase();    // Set up players and map in startup phase
    engine.mainGameLoop();    // Run the main game loop
    std::cout << "Game loop has completed." << std::endl;
}
