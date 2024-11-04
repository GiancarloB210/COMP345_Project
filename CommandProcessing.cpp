//
// Created by Dugua on 11/4/2024.
//

#include "CommandProcessing.h"

#include <iostream>
#include <utility>

Command::Command(std::string command) {
    this->command = command;
}

FileCommandProcessorAdapter::FileCommandProcessorAdapter(std::string fileName) {
    this->fileName = fileName;
}

void Command::saveEffect(std::string effect) {
    this->effect = effect;
}

void CommandProcessor::saveCommand(Command* command) {
    this->commandList.push_back(command);
}

std::string CommandProcessor::readCommand() {
    std::string commandString;
    std::cin >> commandString;
}

Command* CommandProcessor::getCommand() {
    Command* command = new Command(readCommand());
    saveCommand(command);
    return command;
}

bool CommandProcessor::validate(Command* command) {
    //TODO: Link this up with GameEngine
    if(false) {
        command->saveEffect("Invalid command entered.");
    }
}

std::string FileLineReader::readLineFromFile(std::string fileName) {
    std::string line;
    return line;
}

