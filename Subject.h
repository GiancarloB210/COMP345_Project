#ifndef SUBJECT_H
#define SUBJECT_H

#include "Observer.h"
#include <vector>
#include <algorithm>

class Subject {
private:
    std::vector<Observer*> observers;

public:
    // Attach an observer to the subject
    void attach(Observer* observer);

    // Detach an observer from the subject
    void detach(Observer* observer);

    // Notify all attached observers about an update
    void notify(ILoggable &loggable);
};

#endif
