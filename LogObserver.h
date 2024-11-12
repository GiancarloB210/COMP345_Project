#ifndef LOGOBSERVER_H
#define LOGOBSERVER_H

#include "Observer.h"
#include <fstream>
#include <iostream>

class LogObserver : public Observer {
private:
    std::ofstream logFile;

public:
    // Constructor to open the log file
    LogObserver(const std::string& filename) {
        logFile.open(filename, std::ios::app);
        if (!logFile.is_open()) {
            std::cerr << "Failed to open log file: " << filename << std::endl;
        }
    }

    // Destructor to close the log file
    ~LogObserver() {
        if (logFile.is_open()) {
            logFile.close();
        }
    }

    // Override update method to log information from ILoggable objects
    void update(const ILoggable& loggable) override {
        if (logFile.is_open()) {
            logFile << loggable.stringToLog() << std::endl;
        }
    }
};
