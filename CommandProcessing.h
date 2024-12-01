//
// Created by Dugua on 11/4/2024.
//

#ifndef COMMANDPROCESSING_H
#define COMMANDPROCESSING_H
#include <list>
#include <string>
#include <fstream>
#include "GameEngine.h"
#include <ILoggable.h>
#include <iostream>
#include <utility>
#include <sstream>
#include <Subject.h>

// Forward Declarations
class Command;
class CommandProcessor;
class FileLineReader;
class FileCommandProcessorAdapter;
class GameEngine;

// Free functions
std::ostream& operator << (std::ostream& os, const Command& command);
std::ostream& operator << (std::ostream& os, const CommandProcessor& commandProcessor);
std::ostream& operator << (std::ostream& os, const FileLineReader& fileLineReader);
std::ostream& operator << (std::ostream& os, const FileCommandProcessorAdapter& fileCommandProcessorAdapter);

class Command/*: public ILoggable, public Subject*/  {
    // Data Members
    std::string command;
    std::string effect;

public:
    // Constructors
    Command(std::string command);
    Command(Command& command);

    // Operators
    Command& operator=(const Command& command);

    // Deconstructor
    ~Command();

    // Accessors
    std::string getCommand();
    std::string getEffect();

    // Mutators
    void setCommand(std::string command);
    void saveEffect(std::string effect);

    // Methods
    std::string toString();
};

class CommandProcessor/*: public ILoggable, public Subject*/ {
    // Data Members
    std::list<Command*> commandList;
protected:
    GameEngine* gameEngine;

    // Private Methods
    std::string readCommand();
    void saveCommand(Command* command);

public:
    // Constructors
    explicit CommandProcessor(GameEngine* gameEngine);
    CommandProcessor(CommandProcessor& commandProcessor);

    // Operators
    CommandProcessor& operator=(const CommandProcessor& commandProcessor);

    // Deconstructor
    ~CommandProcessor();

    // Accessors
    std::list<Command*> getCommandList();

    // Mutators
    void setCommandList(std::list<Command*> commandList);

    // Public Methods
    Command* getCommand();
    bool validate(std::string commandString);
    std::string toString();
    std::vector<std::vector<std::string>> processTournamentCommand(std::string commandString);
};

class FileLineReader {
    std::string fileName;
    std::ifstream inputFileStream;

public:
    // Constructors
    FileLineReader(std::string filename);
    FileLineReader(FileLineReader& fileLineReader);

    // Operators
    FileLineReader& operator=(const FileLineReader& fileLineReader);

    // Deconstructor
    ~FileLineReader();

    // Accessors
    std::ifstream getFile();
    std::string getFileName();

    // Mutators
    void setFile(std::ifstream file);

    // Methods
    std::string readLineFromFile();
    std::string toString();

};

class FileCommandProcessorAdapter: public CommandProcessor {
    // Data Members
    std::string fileName;
    std::list<Command*> commandList;
    FileLineReader* fileLineReader;
    GameEngine* gameEngine;

    // Private Methods
    std::string readCommand();
    void saveCommand(Command* command);

public:
    // Constructors
    FileCommandProcessorAdapter(std::string fileName, GameEngine* gameEngine);
    FileCommandProcessorAdapter(FileCommandProcessorAdapter& fileCommandProcessor);

    // Operators
    FileCommandProcessorAdapter& operator=(const FileCommandProcessorAdapter& fileCommandProcessor);

    // Deconstructor
    ~FileCommandProcessorAdapter();

    // Accessors
    std::string getFileName();
    std::list<Command*> getCommandList();
    FileLineReader* getFileLineReader();

    // Mutators
    void setFileName(std::string fileName);
    void setCommandList(std::list<Command*> commandList);
    void addCommand(Command* command);
    void setFileLineReader(FileLineReader* fileLineReader);

    // Public Methods
    Command* getCommand();
    bool validate(std::string commandString);
    std::string toString();
    std::vector<std::vector<std::string>> processTournamentCommand(std::string commandString);

};

std::vector<std::vector<std::string>> processTournamentCommandString(std::string commandString);
bool validateTournamentInputs(std::vector<std::vector<std::string>> tournamentInputs);

#endif //COMMANDPROCESSING_H
