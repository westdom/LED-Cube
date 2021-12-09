#ifndef SolidColour_H
#define SolidColour_H

#include "Pattern.h"

// Interface for the SolidColour pattern. Implements the Pattern Interface, and defines what methods are required for "SolidColour.cpp".
class SolidColour : public Pattern
{
    CRGB leds[27];
    String name;
    uint32_t colour;
    boolean rainbowCube;
    const unsigned int DELAY = 50;
    unsigned long previousUpdateStartTime = millis();

public:
    // Fixed colour
    SolidColour(String name, uint32_t colour);
    // Rainbow colour
    SolidColour(String name);
    virtual void update();
    virtual String getName();
};

#endif
