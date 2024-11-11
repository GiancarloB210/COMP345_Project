#ifndef ILOGGABLE_H
#define ILOGGABLE_H

#include <string>

class ILoggable {
public:
    virtual std::string stringToLog() const = 0;  // Pure virtual function for logging
    virtual ~ILoggable() = default;  // Virtual destructor for safe polymorphic deletion
};

#endif
