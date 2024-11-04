//
// Created by Dugua on 11/4/2024.
//

#ifndef COMMANDPROCESSING_H
#define COMMANDPROCESSING_H
#include <list>
#include <string>

class Command {
    std::string command;
    std::string effect;

public:
    Command(std::string command);
    void saveEffect(std::string effect);

};

class CommandProcessor {
    std::string readCommand();
    void saveCommand(Command* command);
    bool validate(Command* command);

    std::list<Command*> commandList;

public:
    Command* getCommand();
};

class FileCommandProcessorAdapter: public CommandProcessor {
    std::string readCommand();
    void saveCommand(Command* command);
    bool validate(Command* command);
    std::string fileName;

    std::list<Command*> commandList;

public:
    FileCommandProcessorAdapter(std::string fileName);
    Command* getCommand();

};

class FileLineReader {
public:
    std::string readLineFromFile(std::string fileName);
};


#endif //COMMANDPROCESSING_H
