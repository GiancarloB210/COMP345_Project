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

// Test the game engine's startup phase
void testStartupPhase() {
    GameEngine engine;
    engine.startupPhase();
    // Print the final state of the game engine
    std::cout << engine << std::endl;
}

// Test the main game loop
void testMainGameLoop() {
    GameEngine engine;
    
    // Perform startup phase to set up players and map
    engine.startupPhase();
    
    // Start the main game loop, which will run until there's a single winner
    engine.mainGameLoop();
    
    // Print the final state of the game engine after the game loop ends
    std::cout << engine << std::endl;
}
