#ifndef OBSERVER_H
#define OBSERVER_H

#include "ILoggable.h"

class Observer {
public:
    virtual void update(ILoggable& loggable) = 0;  // Pure virtual function for updates
    virtual ~Observer() = default;  // Virtual destructor for safe polymorphic deletion
};

#endif
