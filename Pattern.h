#ifndef Pattern_H
#define Pattern_H

#include <Arduino.h>

class Pattern {
    public:
        virtual void update();
        virtual String getType();
};

#endif