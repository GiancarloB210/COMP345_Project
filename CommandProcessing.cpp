//
// Created by Dugua on 11/4/2024.
//

#include "CommandProcessing.h"

// Command Class Constructor
Command::Command(std::string command) {
    this->command = command;
}

// Command Class Copy Constructor
Command::Command(Command& command) {
    this->command = command.command;
    this->effect = command.effect;
}

// Command Class Operators
std::ostream& operator<<(std::ostream& os, Command& command) {
    return (os << "Command: " << command.getCommand() << "\nEffect: " << command.getEffect() << std::endl);
}

Command& Command::operator=(const Command& command) = default;

// Command Class Deconstructor
Command::~Command() = default;

// Command Class Accessors
std::string Command::getCommand() {
    return this->command;
}

std::string Command::getEffect() {
    return this->effect;
}

// Command Class Mutators
void Command::setCommand(std::string command) {
    this->command = command;
}

void Command::saveEffect(std::string effect) {
    this->effect = effect;
}

// Command Class Methods

// toString() uses the overloaded << operator to generate a formatted string containing the values of the
// Command's data members
std::string Command::toString() {
    std::stringstream ss;
    ss << (*this);
    return ss.str();
}

// CommandProcessor Class Constructor
CommandProcessor::CommandProcessor()= default;

// CommandProcessor Class Copy Constructor
CommandProcessor::CommandProcessor(CommandProcessor& commandProcessor) {
    this->commandList = commandProcessor.commandList;
}

// CommandProcessor Class Deconstructors
CommandProcessor::~CommandProcessor() {
    for (auto const& command : this->commandList) {
        delete command;
    }
}

// CommandProcessor Class Operators
std::ostream& operator<<(std::ostream& os, CommandProcessor& commandProcessor) {
    std::string commands;
    for (auto const& i : commandProcessor.getCommandList()) {
        commands.append("\n" + i->toString());
    }

    return (os << "CommandProcessor's list of commands: " << commands << std::endl);
}

CommandProcessor& CommandProcessor::operator=(const CommandProcessor& commandProcessor) = default;

// CommandProcessor Class Accessors
std::list<Command*> CommandProcessor::getCommandList() {
    return this->commandList;
}

// CommandProcessor Class Mutators
void CommandProcessor::setCommandList(std::list<Command*> commandList) {
    this->commandList = commandList;
}

// CommandProcessor Class Private Methods

// readCommand()
std::string CommandProcessor::readCommand() {
    std::string commandString;
    std::cin >> commandString;
    return commandString;
}

// saveCommand() takes in a pointer to a command and adds it to the back of CommandProcessor's commandList
void CommandProcessor::saveCommand(Command* command) {
    this->commandList.push_back(command);
}

// CommandProcessor Class Public Methods

// getCommand() calls the readCommand() private method to create a new command, saves it using the saveCommand
// and returns the newly created command.
Command* CommandProcessor::getCommand() {
    Command* command = new Command(readCommand());
    saveCommand(command);
    return command;
}

// validate
bool CommandProcessor::validate(Command* command) {
    //TODO: Link this up with GameEngine
    if(false) {
        command->saveEffect("Invalid command entered.");
        return false;
    }
    return true;
}

// toString() uses the overloaded << operator to generate a formatted string containing the values of the
// CommandProcessor's data members
std::string CommandProcessor::toString() {
    std::stringstream ss;
    ss << *this;
    return ss.str();
}

// FileLineReader Class Constructor
FileLineReader::FileLineReader(std::string filename) {
    this->inputFileStream = std::ifstream(filename);
}

// FileLineReader Class Copy Constructor
FileLineReader::FileLineReader(FileLineReader& fileLineReader) {
    this->inputFileStream = fileLineReader.getFile();
}

// FileLineReader Class Deconstructor
FileLineReader::~FileLineReader() = default;

// FileLineReader Class Operators
std::ostream& operator<<(std::ostream& os, FileLineReader& fileLineReader) {
    std::string fileName;
    return os << fileLineReader.getFileName() << std::endl;
}

FileLineReader& FileLineReader::operator=(const FileLineReader& fileLineReader) {
    if (this != &fileLineReader) {
        this->fileName = fileLineReader.fileName;
        this->inputFileStream = std::ifstream(fileName);
    }
    return *this;
};

// FileLineReader Class Accessors
std::ifstream FileLineReader::getFile() {
    return std::move(this->inputFileStream);
}

std::string FileLineReader::getFileName() {
    return this->fileName;
}

// FileLineReader Class Mutators
void FileLineReader::setFile(std::ifstream file) {
    this->inputFileStream = std::move(file);
}

// FileLineReader Class Public Methods

// readLineFromFile() opens the inputFileStream and reads/returns the next line while keeping track of
// Where in the file the fileLineReader is
std::string FileLineReader::readLineFromFile() {
    this->inputFileStream.open(fileName);
    static int lineCounter = 0;
    int tempCounter = lineCounter;
    std::string line;

    // Check if the file at the provided filePath can be opened
    if (!inputFileStream.is_open()) {
        std::cout << "Error opening file " << fileName << std::endl;
        return "";
    }

    while(tempCounter != 0) {
        std::getline(inputFileStream,line);
        tempCounter--;
    }
    while(std::getline(inputFileStream,line)) {
        lineCounter++;
        if (!line.empty())
            break;
    }
    inputFileStream.close();
    return line;
}

// toString() uses the overloaded << operator to generate a formatted string containing the values of the
// FileLineReader's data members
std::string FileLineReader::toString() {
    std::stringstream ss;
    ss << *this;
    return ss.str();
}

// FileCommandProcessorAdapter Class Constructor
FileCommandProcessorAdapter::FileCommandProcessorAdapter(std::string fileName) {
    this->fileName = fileName;
    this->fileLineReader = new FileLineReader(fileName);
}

// FileCommandProcessorAdapter Class Copy Constructor
FileCommandProcessorAdapter::FileCommandProcessorAdapter(FileCommandProcessorAdapter& fileCommanProcessorAdapter) {
    this->fileName = fileCommanProcessorAdapter.fileName;
    this->fileLineReader = fileCommanProcessorAdapter.fileLineReader;
}

// FileCommandProcessorAdapter Class Deconstructor
FileCommandProcessorAdapter::~FileCommandProcessorAdapter() {
    for (auto const& i : this->commandList) {
        delete i;
    }
    delete fileLineReader;
    fileLineReader = nullptr;
}

// FileCommandProcessorAdapter Class Operators
FileCommandProcessorAdapter& FileCommandProcessorAdapter::operator=(const FileCommandProcessorAdapter& fileCommandProcessorAdapter) = default;

std::ostream& operator<<(std::ostream& os, FileCommandProcessorAdapter& fileCommandProcessorAdapter) {
    std::string commands;

    for (auto const& i : fileCommandProcessorAdapter.getCommandList()) {
        commands.append("\n" + i->toString());
    }
    return os << "FileCommandProcessorAdapter:" << "\nFile Name: " << fileCommandProcessorAdapter.getFileName()
    << "\nCommand List:" << commands << std::endl;
}

// FileCommandProcessorAdapter CLass Accessors
std::string FileCommandProcessorAdapter::getFileName() {
    return this->fileName;
}

std::list<Command*> FileCommandProcessorAdapter::getCommandList() {
    return this->commandList;
}

FileLineReader* FileCommandProcessorAdapter::getFileLineReader() {
    return this->fileLineReader;
}

// FileCommandProcessorAdapter Class Mutators
void FileCommandProcessorAdapter::setFileName(std::string fileName) {
    this->fileName = fileName;
}

void FileCommandProcessorAdapter::setCommandList(std::list<Command *> commandList) {
    this->commandList = commandList;
}

void FileCommandProcessorAdapter::addCommand(Command *command) {
    this->commandList.push_back(command);
}

void FileCommandProcessorAdapter::setFileLineReader(FileLineReader *fileLineReader) {
    this->fileLineReader = fileLineReader;
}

// FileCommandProcessorAdapter Class Private Methods

// readCommand() returns the next line from the commandsFile using the readLineFromFile() from fileLineReader
std::string FileCommandProcessorAdapter::readCommand() {
    return this->fileLineReader->readLineFromFile();
}

// saveCommand() takes in a pointer to a command and adds it to the back of FileCommandProcessorAdapter's commandList
void FileCommandProcessorAdapter::saveCommand(Command* command) {
    this->commandList.push_back(command);
}

// FileCommandProcessorAdapter Class Public Methods

Command* FileCommandProcessorAdapter::getCommand() {
    Command* command = new Command(readCommand());
    saveCommand(command);
    return command;
}

bool FileCommandProcessorAdapter::validate(Command* command) {
    // TODO: Link with gameEngine
    if(false) {
        command->saveEffect("Invalid command entered.");
        return false;
    }
    return true;
}

// toString() uses the overloaded << operator to generate a formatted string containing the values of the
// FileCommandProcessorAdapter's data members
std::string FileCommandProcessorAdapter::toString() {
    std::stringstream ss;
    ss << *this;
    return ss.str();
}