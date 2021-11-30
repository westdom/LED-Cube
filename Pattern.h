#ifndef Pattern_H
#define Pattern_H

#include <Arduino.h>

// Interface that forces classes which implement it to have the "update" and "getName" functions.
class Pattern
{
    // Public means if you have a reference to an object of type Pattern, you can invoke the below functions on that object.
public:
    // Virtual forces classes that implement this interface to override these functions.
    virtual ~Pattern() = default;
    virtual void update();
    virtual String getName();
};

#endif