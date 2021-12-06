#include "SolidColour.h"

SolidColour::SolidColour(String name)
{
    this->name = name;
    this->colour = 0x000000;
    this->rainbowCube = true;
    setup();
}

SolidColour::SolidColour(String name, uint32_t colour)
{
    this->name = name;
    this->colour = colour;
    this->rainbowCube = false;
    setup();
}

void SolidColour::setup()
{
    FastLED.addLeds<NEOPIXEL, LED_PIN>(leds, 27);
    FastLED.setBrightness(MAX_BRIGHTNESS);
}

String SolidColour::getName()
{
    return name;
}

void SolidColour::update()
{
    if (rainbowCube)
    {
        colour = getNextRainbowColour();
    }
    for (int i = 0; i < 27; i++)
    {
        sendColour(leds, i, colour);
    }
    FastLED.show();
}

int SolidColour::getDelayMultiplier()
{
    return 1;
}
