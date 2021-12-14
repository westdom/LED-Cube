#ifndef Walls_H
#define Walls_H

#include "Pattern.h"

// Interface for the SolidColour pattern. Implements the Pattern Interface, and defines what methods are required for "SolidColour.cpp".
class Walls : public Pattern
{
    CRGB leds[27];
    String name;
    uint32_t colour;
    unsigned long previousUpdateStartTime = millis();

public:
    Walls(String name);
    virtual void update();
    virtual String getName();

private:
    virtual void resetColours();
};

#endif
