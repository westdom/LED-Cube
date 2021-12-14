#include "RotatingCube.h"

const int CORNERS[][3] = {{0, 0, 0}, {2, 0, 0}, {2, 0, 2}, {0, 0, 2}, {0, 2, 2}, {2, 2, 2}, {2, 2, 0}, {0, 2, 0}};
const unsigned int ROTATING_CUBE_DELAY = 200;
const unsigned int CORNER_CHANGE_DELAY = 1000;

unsigned int rotatingCubeCurrentDelay = CORNER_CHANGE_DELAY;
int cornerIndex = 0;
int animationPhase = 0;
uint32_t rotatingCubeColour;
uint32_t rotatingCubeColours[3][3][3] = {
    {{CRGB::Black, CRGB::Black, CRGB::Black},
     {CRGB::Black, CRGB::Black, CRGB::Black},
     {CRGB::Black, CRGB::Black, CRGB::Black}},
    {{CRGB::Black, CRGB::Black, CRGB::Black},
     {CRGB::Black, CRGB::Black, CRGB::Black},
     {CRGB::Black, CRGB::Black, CRGB::Black}},
    {{CRGB::Black, CRGB::Black, CRGB::Black},
     {CRGB::Black, CRGB::Black, CRGB::Black},
     {CRGB::Black, CRGB::Black, CRGB::Black}}};

RotatingCube::RotatingCube(String name)
{
    this->name = name;
    FastLED.addLeds<NEOPIXEL, LED_PIN>(leds, 27);
    FastLED.setBrightness(MAX_BRIGHTNESS);
    rotatingCubeColour = getNextRainbowColour();
}

String RotatingCube::getName()
{
    return name;
}

void RotatingCube::update()
{
    const unsigned long currentTime = millis();
    sendColours(leds, rotatingCubeColours);
    if (currentTime - previousUpdateStartTime >= rotatingCubeCurrentDelay)
    {
        rotatingCubeColour = getNextRainbowColour();
        previousUpdateStartTime = currentTime;

        const int currentCornerX = CORNERS[cornerIndex][0];
        const int currentCornerY = CORNERS[cornerIndex][1];
        const int currentCornerZ = CORNERS[cornerIndex][2];

        switch (animationPhase)
        {
        case 0:
            setRotatingCubeColours(CRGB::Black, 2);
            setRotatingCubeColours(rotatingCubeColour, 1);
            rotatingCubeCurrentDelay = CORNER_CHANGE_DELAY;
            animationPhase++;
            break;
        case 1:
            rotatingCubeCurrentDelay = ROTATING_CUBE_DELAY;
            setRotatingCubeColours(CRGB::Black, 1);
            rotatingCubeColours[currentCornerX][currentCornerY][currentCornerZ] = rotatingCubeColour;
            animationPhase++;
            break;
        case 2:
            setRotatingCubeColours(rotatingCubeColour, 1);
            animationPhase++;
            break;
        case 3:
            setRotatingCubeColours(rotatingCubeColour, 2);
            cornerIndex++;
            if(cornerIndex >= 8) {
                cornerIndex = 0;
            }
            animationPhase = 0;
            break;
        }
    }
}

void RotatingCube::setRotatingCubeColours(uint32_t rotatingCubeColour, int distanceFromCorner)
{
    for (int x = CORNERS[cornerIndex][0] - distanceFromCorner; x <= CORNERS[cornerIndex][0] + distanceFromCorner; x++)
    {
        for (int y = CORNERS[cornerIndex][1] - distanceFromCorner; y <= CORNERS[cornerIndex][1] + distanceFromCorner; y++)
        {
            for (int z = CORNERS[cornerIndex][2] - distanceFromCorner; z <= CORNERS[cornerIndex][2] + distanceFromCorner; z++)
            {
                if (isPositionWithinCubesBorders(x) && isPositionWithinCubesBorders(y) && isPositionWithinCubesBorders(z))
                {
                    rotatingCubeColours[x][y][z] = rotatingCubeColour;
                }
            }
        }
    }
}

bool RotatingCube::isPositionWithinCubesBorders(int pos)
{
    return pos >= CUBE_AXIS_LOWER_BOUND && pos <= CUBE_AXIS_UPPER_BOUND;
}
