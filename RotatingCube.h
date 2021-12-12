#ifndef RotatingCube_H
#define RotatingCube_H

#include "Pattern.h"

// Interface for the SolidColour pattern. Implements the Pattern Interface, and defines what methods are required for "SolidColour.cpp".
class RotatingCube : public Pattern
{
    CRGB leds[27];
    String name;
    uint32_t colour;
    unsigned long previousUpdateStartTime = millis();

public:
    RotatingCube(String name);
    virtual void update();
    virtual String getName();

private:
    virtual bool isPositionWithinCubesBorders(int pos);
    virtual void setRotatingCubeColours(uint32_t colour, int distance);
};

#endif
