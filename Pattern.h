#ifndef Pattern_H
#define Pattern_H

#include <Arduino.h>

// Interface that forces classes which implement it must have the "update" and "getName" functions.
class Pattern {
    public:
        virtual void update();
        virtual String getName();
};

#endif