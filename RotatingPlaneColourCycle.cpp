#include "RotatingPlaneColourCycle.h"

RotatingPlaneColourCycle::RotatingPlaneColourCycle(String name)
{
    this->name = name;
    setup();
}

void RotatingPlaneColourCycle::setup()
{
    FastLED.addLeds<NEOPIXEL, LED_PIN>(leds, 27);
    FastLED.setBrightness(MAX_BRIGHTNESS);
}

String RotatingPlaneColourCycle::getName()
{
    return name;
}

int RotatingPlaneColourCycle::getDelayMultiplier()
{
    return 1;
}

void RotatingPlaneColourCycle::update()
{
    const uint32_t COLOUR = getNextRainbowColour();
    const uint32_t BLACK = 0x000000;
    static int iteration = 0;
    int ledIndex = 0;
    switch (iteration)
    {
    case 0:
        // 0
        sendColour(leds, ledIndex++, BLACK);
        sendColour(leds, ledIndex++, BLACK);
        sendColour(leds, ledIndex++, BLACK);

        sendColour(leds, ledIndex++, BLACK);
        sendColour(leds, ledIndex++, BLACK);
        sendColour(leds, ledIndex++, BLACK);

        sendColour(leds, ledIndex++, BLACK);
        sendColour(leds, ledIndex++, BLACK);
        sendColour(leds, ledIndex++, BLACK);

        // 1
        sendColour(leds, ledIndex++, COLOUR);
        sendColour(leds, ledIndex++, COLOUR);
        sendColour(leds, ledIndex++, COLOUR);

        sendColour(leds, ledIndex++, COLOUR);
        sendColour(leds, ledIndex++, BLACK);
        sendColour(leds, ledIndex++, COLOUR);

        sendColour(leds, ledIndex++, COLOUR);
        sendColour(leds, ledIndex++, COLOUR);
        sendColour(leds, ledIndex++, COLOUR);

        // 2
        sendColour(leds, ledIndex++, BLACK);
        sendColour(leds, ledIndex++, BLACK);
        sendColour(leds, ledIndex++, BLACK);

        sendColour(leds, ledIndex++, BLACK);
        sendColour(leds, ledIndex++, BLACK);
        sendColour(leds, ledIndex++, BLACK);

        sendColour(leds, ledIndex++, BLACK);
        sendColour(leds, ledIndex++, BLACK);
        sendColour(leds, ledIndex++, BLACK);
        break;
    case 1:
        // 0
        sendColour(leds, ledIndex++, COLOUR);
        sendColour(leds, ledIndex++, BLACK);
        sendColour(leds, ledIndex++, BLACK);

        sendColour(leds, ledIndex++, BLACK);
        sendColour(leds, ledIndex++, BLACK);
        sendColour(leds, ledIndex++, COLOUR);

        sendColour(leds, ledIndex++, COLOUR);
        sendColour(leds, ledIndex++, BLACK);
        sendColour(leds, ledIndex++, BLACK);

        // 1
        sendColour(leds, ledIndex++, BLACK);
        sendColour(leds, ledIndex++, COLOUR);
        sendColour(leds, ledIndex++, BLACK);

        sendColour(leds, ledIndex++, BLACK);
        sendColour(leds, ledIndex++, BLACK);
        sendColour(leds, ledIndex++, BLACK);

        sendColour(leds, ledIndex++, BLACK);
        sendColour(leds, ledIndex++, COLOUR);
        sendColour(leds, ledIndex++, BLACK);

        // 2
        sendColour(leds, ledIndex++, BLACK);
        sendColour(leds, ledIndex++, BLACK);
        sendColour(leds, ledIndex++, COLOUR);

        sendColour(leds, ledIndex++, COLOUR);
        sendColour(leds, ledIndex++, BLACK);
        sendColour(leds, ledIndex++, BLACK);

        sendColour(leds, ledIndex++, BLACK);
        sendColour(leds, ledIndex++, BLACK);
        sendColour(leds, ledIndex++, COLOUR);
        break;
    case 2:
        // 0
        sendColour(leds, ledIndex++, BLACK);
        sendColour(leds, ledIndex++, COLOUR);
        sendColour(leds, ledIndex++, BLACK);

        sendColour(leds, ledIndex++, BLACK);
        sendColour(leds, ledIndex++, COLOUR);
        sendColour(leds, ledIndex++, BLACK);

        sendColour(leds, ledIndex++, BLACK);
        sendColour(leds, ledIndex++, COLOUR);
        sendColour(leds, ledIndex++, BLACK);

        // 1
        sendColour(leds, ledIndex++, BLACK);
        sendColour(leds, ledIndex++, COLOUR);
        sendColour(leds, ledIndex++, BLACK);

        sendColour(leds, ledIndex++, BLACK);
        sendColour(leds, ledIndex++, BLACK);
        sendColour(leds, ledIndex++, BLACK);

        sendColour(leds, ledIndex++, BLACK);
        sendColour(leds, ledIndex++, COLOUR);
        sendColour(leds, ledIndex++, BLACK);

        // 2
        sendColour(leds, ledIndex++, BLACK);
        sendColour(leds, ledIndex++, COLOUR);
        sendColour(leds, ledIndex++, BLACK);

        sendColour(leds, ledIndex++, BLACK);
        sendColour(leds, ledIndex++, COLOUR);
        sendColour(leds, ledIndex++, BLACK);

        sendColour(leds, ledIndex++, BLACK);
        sendColour(leds, ledIndex++, COLOUR);
        sendColour(leds, ledIndex++, BLACK);
        break;
    case 3:
        // 0
        sendColour(leds, ledIndex++, BLACK);
        sendColour(leds, ledIndex++, BLACK);
        sendColour(leds, ledIndex++, COLOUR);

        sendColour(leds, ledIndex++, COLOUR);
        sendColour(leds, ledIndex++, BLACK);
        sendColour(leds, ledIndex++, BLACK);

        sendColour(leds, ledIndex++, BLACK);
        sendColour(leds, ledIndex++, BLACK);
        sendColour(leds, ledIndex++, COLOUR);

        // 1
        sendColour(leds, ledIndex++, BLACK);
        sendColour(leds, ledIndex++, COLOUR);
        sendColour(leds, ledIndex++, BLACK);

        sendColour(leds, ledIndex++, BLACK);
        sendColour(leds, ledIndex++, BLACK);
        sendColour(leds, ledIndex++, BLACK);

        sendColour(leds, ledIndex++, BLACK);
        sendColour(leds, ledIndex++, COLOUR);
        sendColour(leds, ledIndex++, BLACK);

        // 2
        sendColour(leds, ledIndex++, COLOUR);
        sendColour(leds, ledIndex++, BLACK);
        sendColour(leds, ledIndex++, BLACK);

        sendColour(leds, ledIndex++, BLACK);
        sendColour(leds, ledIndex++, BLACK);
        sendColour(leds, ledIndex++, COLOUR);

        sendColour(leds, ledIndex++, COLOUR);
        sendColour(leds, ledIndex++, BLACK);
        sendColour(leds, ledIndex++, BLACK);
        break;
    }
    iteration++;
    if (iteration > 3)
    {
        iteration = 0;
    }
    FastLED.show();
}
