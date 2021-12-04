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
        shiftColour();
    }
    for (int i = 0; i < 27; i++)
    {
        leds[i].red = rgbColour[0];
        leds[i].green = rgbColour[1];
        leds[i].blue = rgbColour[2];
    }
    FastLED.show();
}

void SolidColour::shiftColour()
{
    static unsigned int rgbColour[3];
    static int decColour = 0;
    static int incColour = 2;
    static int colourIndex = 0;

    if (colourIndex == 0)
    {
        // Reset rgbColours
        rgbColour[0] = 0;
        rgbColour[1] = 0;
        rgbColour[2] = 0;
    }

    colourIndex++;
    if (colourIndex == MAX_BRIGHTNESS)
    {
        colourIndex = 0;

        // Shift dec & inc colours to next rgb byte
        decColour++;
        incColour--;
        if (decColour > 2 || incColour < 0)
        {
            decColour = 0;
            incColour = 2;
        }
    }
    // Increment/decrement respective bytes of rgbColour
    const static int INCREMENT_BY = 10;
    rgbColour[decColour] -= INCREMENT_BY;
    rgbColour[incColour] += INCREMENT_BY;
}

int SolidColour::findIndexOfString(const String STRING, const String arr[], const int ARR_SIZE)
{
    for (int i = 0; i < ARR_SIZE; i++)
    {
        if (arr[i].equals(STRING))
        {
            return i;
        }
    }
    return -1;
}