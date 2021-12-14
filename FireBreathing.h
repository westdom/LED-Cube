#ifndef FireBreathing_H
#define FireBreathing_H

#include "Pattern.h"

// Interface for the SolidColour pattern. Implements the Pattern Interface, and defines what methods are required for "SolidColour.cpp".
class FireBreathing : public Pattern
{
    CRGB leds[27];
    String name;
    uint32_t colour;

public:
    FireBreathing(String name);
    virtual void update();
    virtual String getName();
};

#endif
