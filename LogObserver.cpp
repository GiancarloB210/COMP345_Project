#include "LogObserver.h"
#include <iostream>

LogObserver::LogObserver(const std::string& filename) {
    logFile.open(filename, std::ios::app);
    if (!logFile.is_open()) {
        std::cerr << "Failed to open log file: " << filename << std::endl;
    }
}

LogObserver::~LogObserver() {
    if (logFile.is_open()) {
        logFile.close();
    }
}

void LogObserver::update(const ILoggable& loggable) {
    if (logFile.is_open()) {
        logFile << loggable.stringToLog() << std::endl;
    }
}