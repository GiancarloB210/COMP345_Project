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
    LogObserver(const std::string& filename);

    // Destructor to close the log file
    ~LogObserver();

    // Override update method to log information from ILoggable objects
    void update(ILoggable& loggable) override;
};

#endif
