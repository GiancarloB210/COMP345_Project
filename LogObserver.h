#ifndef LOGOBSERVER_H
#define LOGOBSERVER_H

#include "Observer.h"
#include <fstream>

class LogObserver : public Observer {
private:
    std::ofstream logFile;

public:
    LogObserver(const std::string& filename);
    ~LogObserver();
    void update(ILoggable& loggable) override;
};

#endif
