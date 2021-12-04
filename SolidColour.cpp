#include "SolidColour.h"

// Constants
const int LED_PIN = 3;
const static int MAX_BRIGHTNESS = 50;
const String COLOURS[8] = {"BLACK", "WHITE", "RED", "LIME", "BLUE", "YELLOW", "CYAN", "MAGENTA"};
const int RGB_COLOUR_VALUES[8][3] = {{0, 0, 0}, {MAX_BRIGHTNESS, MAX_BRIGHTNESS, MAX_BRIGHTNESS}, {MAX_BRIGHTNESS, 0, 0}, {0, MAX_BRIGHTNESS, 0}, {0, 0, MAX_BRIGHTNESS}, {MAX_BRIGHTNESS, MAX_BRIGHTNESS, 0}, {0, MAX_BRIGHTNESS, MAX_BRIGHTNESS}, {MAX_BRIGHTNESS, 0, MAX_BRIGHTNESS}};

// Variables
int rgbColour[3] = {0, 0, 0};

SolidColour::SolidColour(String name)
{
    this->name = name;
    this->colour = "BLACK";
    this->rainbowCube = true;
    setup();
}

SolidColour::SolidColour(String name, String colour)
{
    this->name = name;
    this->colour = colour;
    this->rainbowCube = false;
    setup();
}

void SolidColour::setup()
{
    FastLED.addLeds<NEOPIXEL, LED_PIN>(leds, 27);
    {
        for (int i = 0; i < 3; i++)
        {
            rgbColour[i] = RGB_COLOUR_VALUES[findIndexOfString(colour, COLOURS, 16)][i];
        }
    }
}

String SolidColour::getName()
{
    return name;
}

void SolidColour::update()
{
    if (rainbowCube)
    {
        shiftColour(rgbColour, MAX_BRIGHTNESS);
    }
    for (int i = 0; i < 27; i++)
    {
        leds[i].red = rgbColour[0];
        leds[i].green = rgbColour[1];
        leds[i].blue = rgbColour[2];
    }
    FastLED.show();
}