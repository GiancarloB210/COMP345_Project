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
    void attach(Observer* observer) {
        observers.push_back(observer);
    }

    // Detach an observer from the subject
    void detach(Observer* observer) {
        observers.erase(std::remove(observers.begin(), observers.end(), observer), observers.end());
    }

    // Notify all attached observers about an update
    void notify(const ILoggable& loggable) {
        for (Observer* observer : observers) {
            observer->update(loggable);
        }
    }
};

#endif
