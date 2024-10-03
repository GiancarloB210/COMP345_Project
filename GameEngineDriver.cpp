#include "GameEngine.h"
#include <iostream>
#include <string>

// Testing game states and allow user to interact via console
void testGameStates() {
    GameEngine engine; 
    std::string command;

    // Looping to accept user commands and process them
    while (true) {
        std::cout << "Enter command (type 'exit' to stop): ";
        std::cin >> command;

        // Break the loop if the user wants to exit
        if (command == "exit") {
            break;
        }

        
        engine.handleCommand(command);
    }
}


int main() {
    testGameStates();  // Call the test function
    return 0;
}
