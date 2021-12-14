#ifndef RotatingPlaneColourCycle_H
#define RotatingPlaneColourCycle_H

#include "Pattern.h"

// Interface for the SolidColour pattern. Implements the Pattern Interface, and defines what methods are required for "SolidColour.cpp".
class RotatingPlaneColourCycle : public Pattern
{
    CRGB leds[27];
    String name;
    const unsigned int DELAY = 150;
    unsigned long previousUpdateStartTime = millis();

public:
    RotatingPlaneColourCycle(String name);
    virtual void update();
    virtual String getName();
    virtual int getDelayMultiplier();

private:
    virtual void setup();
};

#endif
