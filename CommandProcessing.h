//
// Created by Dugua on 11/4/2024.
//

#ifndef COMMANDPROCESSING_H
#define COMMANDPROCESSING_H
#include <list>
#include <string>
#include <fstream>
#include <iostream>
#include <utility>
#include <sstream>

// Forward Declarations
class Command;
class CommandProcessor;
class FileLineReader;
class FileCommandProcessorAdapter;

// Free functions
std::ostream& operator << (std::ostream& os, const Command& command);
std::ostream& operator << (std::ostream& os, const CommandProcessor& commandProcessor);
std::ostream& operator << (std::ostream& os, const FileLineReader& fileLineReader);
std::ostream& operator << (std::ostream& os, const FileCommandProcessorAdapter& fileCommandProcessorAdapter);

class Command {
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

class CommandProcessor {
    // Data Members
    std::list<Command*> commandList;

    // Private Methods
    std::string readCommand();
    void saveCommand(Command* command);

public:
    // Constructors
    CommandProcessor();
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
    bool validate(Command* command);
    std::string toString();
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

    // Private Methods
    std::string readCommand();
    void saveCommand(Command* command);
    bool validate(Command* command);

public:
    // Constructors
    FileCommandProcessorAdapter(std::string fileName);
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

};


#endif //COMMANDPROCESSING_H
