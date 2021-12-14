#include "Walls.h"

const unsigned int wallsDelay = 200;
int wallsAxis = 0;
int wallsStage = 0;

uint32_t wallsColour;
uint32_t wallsColours[3][3][3] = {
    {{CRGB::Black, CRGB::Black, CRGB::Black},
     {CRGB::Black, CRGB::Black, CRGB::Black},
     {CRGB::Black, CRGB::Black, CRGB::Black}},
    {{CRGB::Black, CRGB::Black, CRGB::Black},
     {CRGB::Black, CRGB::Black, CRGB::Black},
     {CRGB::Black, CRGB::Black, CRGB::Black}},
    {{CRGB::Black, CRGB::Black, CRGB::Black},
     {CRGB::Black, CRGB::Black, CRGB::Black},
     {CRGB::Black, CRGB::Black, CRGB::Black}}};

Walls::Walls(String name)
{
    this->name = name;
    FastLED.addLeds<NEOPIXEL, LED_PIN>(leds, 27);
    FastLED.setBrightness(MAX_BRIGHTNESS);
    wallsColour = getNextRainbowColour();
}

String Walls::getName()
{
    return name;
}

void Walls::update()
{
    const unsigned long currentTime = millis();
    if (currentTime - previousUpdateStartTime >= wallsDelay)
    {
        previousUpdateStartTime = currentTime;
        wallsColour = getNextRainbowColour();
        switch (wallsAxis)
        {
        case 0:
            resetColours();
            for (int y = 0; y < 3; y++)
            {
                wallsColours[wallsStage < 3 ? wallsStage : 5 - wallsStage][y][0] = wallsColour;
                wallsColours[wallsStage < 3 ? wallsStage : 5 - wallsStage][y][1] = wallsColour;
                wallsColours[wallsStage < 3 ? wallsStage : 5 - wallsStage][y][2] = wallsColour;
            }
            wallsStage++;
            if (wallsStage == 6)
            {
                wallsAxis++;
                wallsStage = 0;
            }
            break;
        case 1:
            resetColours();
            for (int z = 0; z < 3; z++)
            {
                wallsColours[0][wallsStage < 3 ? wallsStage : 5 - wallsStage][z] = wallsColour;
                wallsColours[1][wallsStage < 3 ? wallsStage : 5 - wallsStage][z] = wallsColour;
                wallsColours[2][wallsStage < 3 ? wallsStage : 5 - wallsStage][z] = wallsColour;
            }
            wallsStage++;
            if (wallsStage == 6)
            {
                wallsAxis++;
                wallsStage = 0;
            }
            break;
        case 2:
            resetColours();
            for (int x = 0; x < 3; x++)
            {
                wallsColours[x][0][wallsStage < 3 ? wallsStage : 5 - wallsStage] = wallsColour;
                wallsColours[x][1][wallsStage < 3 ? wallsStage : 5 - wallsStage] = wallsColour;
                wallsColours[x][2][wallsStage < 3 ? wallsStage : 5 - wallsStage] = wallsColour;
            }
            wallsStage++;
            if (wallsStage == 6)
            {
                wallsAxis = 0;
                wallsStage = 0;
            }
            break;
        }
        sendColours(leds, wallsColours);
    }
}

void Walls::resetColours()
{
    for (int x = 0; x < 3; x++)
    {
        for (int y = 0; y < 3; y++)
        {
            for (int z = 0; z < 3; z++)
            {
                wallsColours[x][y][z] = CRGB::Black;
            }
        }
    }
}