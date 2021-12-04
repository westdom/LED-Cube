#ifndef SolidColour_H
#define SolidColour_H

#include "Pattern.h"

// Interface for the SolidColour pattern. Implements the Pattern Interface, and defines what methods are required for "SolidColour.cpp".
class SolidColour : public Pattern
{
    CRGB leds[27];
    String name;
    String colour;
    boolean rainbowCube;

public:
    // Fixed colour
    SolidColour(String name, String colour);
    // Rainbow colour
    SolidColour(String name);
    virtual void update();
    virtual String getName();

private:
    virtual void setup();
};

#endif
