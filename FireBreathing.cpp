#include "FireBreathing.h"

const long maxDelay = 500;
long fireBreathingDelays[3][3][3] = {{{random(0, maxDelay), random(0, maxDelay), random(0, maxDelay)},
                                     {random(0, maxDelay), random(0, maxDelay), random(0, maxDelay)},
                                     {random(0, maxDelay), random(0, maxDelay), random(0, maxDelay)}},
                                    {{random(0, maxDelay), random(0, maxDelay), random(0, maxDelay)},
                                     {random(0, maxDelay), random(0, maxDelay), random(0, maxDelay)},
                                     {random(0, maxDelay), random(0, maxDelay), random(0, maxDelay)}},
                                    {{random(0, maxDelay), random(0, maxDelay), random(0, maxDelay)},
                                     {random(0, maxDelay), random(0, maxDelay), random(0, maxDelay)},
                                     {random(0, maxDelay), random(0, maxDelay), random(0, maxDelay)}}};
unsigned long previousUpdateStartTime[3][3][3] = {{{millis(), millis(), millis()},
                                                   {millis(), millis(), millis()},
                                                   {millis(), millis(), millis()}},
                                                  {{millis(), millis(), millis()},
                                                   {millis(), millis(), millis()},
                                                   {millis(), millis(), millis()}},
                                                  {{millis(), millis(), millis()},
                                                   {millis(), millis(), millis()},
                                                   {millis(), millis(), millis()}}};
uint32_t fireBreathingColours[3][3][3] = {
    {{CRGB::Black, CRGB::Black, CRGB::Black},
     {CRGB::Black, CRGB::Black, CRGB::Black},
     {CRGB::Black, CRGB::Black, CRGB::Black}},
    {{CRGB::Black, CRGB::Black, CRGB::Black},
     {CRGB::Black, CRGB::Black, CRGB::Black},
     {CRGB::Black, CRGB::Black, CRGB::Black}},
    {{CRGB::Black, CRGB::Black, CRGB::Black},
     {CRGB::Black, CRGB::Black, CRGB::Black},
     {CRGB::Black, CRGB::Black, CRGB::Black}}};

FireBreathing::FireBreathing(String name)
{
    this->name = name;
    FastLED.addLeds<NEOPIXEL, LED_PIN>(leds, 27);
    FastLED.setBrightness(MAX_BRIGHTNESS);
    randomSeed(analogRead(0));
}

String FireBreathing::getName()
{
    return name;
}

void FireBreathing::update()
{
    const unsigned long currentTime = millis();
    for (int x = 0; x < 3; x++)
    {
        for (int y = 0; y < 3; y++)
        {
            for (int z = 0; z < 3; z++)
            {
                if (currentTime - previousUpdateStartTime[x][y][z] >= (unsigned long) fireBreathingDelays[x][y][z])
                {
                    previousUpdateStartTime[x][y][z] = currentTime;
                    fireBreathingDelays[x][y][z] = random(0, maxDelay);
                    int red = random(0, 120) + 135;
                    int green = random(0, 10);
                    int blue = random(0, 10);
                    uint32_t hexColour = ((red & 0xFFL) << 16L) + ((green & 0xFFL) << 8L) + (blue & 0xFFL);
                    fireBreathingColours[x][y][z] = hexColour;
                }
            }
        }
    }
    sendColours(leds, fireBreathingColours);
}
