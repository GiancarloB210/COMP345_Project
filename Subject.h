#ifndef SUBJECT_H
#define SUBJECT_H

#include "Observer.h"
#include <vector>

class Subject {
private:
    std::vector<Observer*> observers;

public:
    void attach(Observer* observer);
    void detach(Observer* observer);
    void notify(ILoggable &loggable);
};

#endif
