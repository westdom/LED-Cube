#include "SolidColour.h"

SolidColour::SolidColour(String name)
{
    this->name = name;
    this->colour = CRGB::Black;
    this->rainbowCube = true;
    FastLED.addLeds<NEOPIXEL, LED_PIN>(leds, 27);
    FastLED.setBrightness(MAX_BRIGHTNESS);
}

SolidColour::SolidColour(String name, uint32_t colour)
{
    this->name = name;
    this->colour = colour;
    this->rainbowCube = false;
    FastLED.addLeds<NEOPIXEL, LED_PIN>(leds, 27);
    FastLED.setBrightness(MAX_BRIGHTNESS);
}

String SolidColour::getName()
{
    return name;
}

void SolidColour::update()
{
    const unsigned long currentTime = millis();
    if (currentTime - previousUpdateStartTime >= DELAY)
    {
        previousUpdateStartTime = currentTime;
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
}
