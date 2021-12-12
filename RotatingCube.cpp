#include "RotatingCube.h"

const int CORNERS[][3] = {{0, 0, 0}, {2, 0, 0}, {2, 0, 2}, {0, 0, 2}, {0, 2, 2}, {2, 2, 2}, {2, 2, 0}, {0, 2, 0}};
const unsigned int NUMBER_OF_HUE_CHANGES = 101;
const unsigned int HUE_DELAY = 10;
const unsigned int ROTATING_CUBE_DELAY = 200;
const unsigned int CORNER_CHANGE_DELAY = 100;

unsigned int currentDelay = ROTATING_CUBE_DELAY;
int cornerIndex = 0;
int animationPhase = 0;
uint32_t colour;
uint32_t colours[3][3][3] = {
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
    colour = getNextRainbowColour();
}

String RotatingCube::getName()
{
    return name;
}

void RotatingCube::update()
{
    const unsigned long currentTime = millis();
    sendColours(leds, colours);
    if (currentTime - previousUpdateStartTime >= currentDelay)
    {
        previousUpdateStartTime = currentTime;

        const int currentCornerX = CORNERS[cornerIndex][0];
        const int currentCornerY = CORNERS[cornerIndex][1];
        const int currentCornerZ = CORNERS[cornerIndex][2];

        switch (animationPhase)
        {
        case 0:
            currentDelay = ROTATING_CUBE_DELAY;
            setRotatingCubeColours(CRGB::Black, 2);
            colours[currentCornerX][currentCornerY][currentCornerZ] = colour;
            animationPhase++;
            break;
        case 1:
            setRotatingCubeColours(colour, 1);
            animationPhase++;
            break;
        case 2:
            setRotatingCubeColours(colour, 2);
            animationPhase++;
            break;
        case 3:
            setRotatingCubeColours(CRGB::Black, 2);
            setRotatingCubeColours(colour, 1);
            animationPhase++;
            break;
        case 4:
            setRotatingCubeColours(CRGB::Black, 1);
            colours[currentCornerX][currentCornerY][currentCornerZ] = colour;
            animationPhase++;
            break;
        case 5:
            static int noTimesShifted = 0;
            currentDelay = HUE_DELAY;
            noTimesShifted++;
            if (noTimesShifted > NUMBER_OF_HUE_CHANGES)
            {
                noTimesShifted = 0;
                animationPhase++;
                currentDelay = CORNER_CHANGE_DELAY;
            }
            colour = getNextRainbowColour();
            colours[currentCornerX][currentCornerY][currentCornerZ] = colour;
            break;
        case 6:
            colours[currentCornerX][currentCornerY][currentCornerZ] = CRGB::Black;
            cornerIndex++;
            if(cornerIndex >= 8) {
                cornerIndex = 0;
            }
            if (currentCornerX != CORNERS[cornerIndex][0])
            {
                for (int x = currentCornerX - 1; x <= currentCornerX + 1; x += 2)
                {
                    if (isPositionWithinCubesBorders(x))
                    {
                        colours[x][currentCornerY][currentCornerZ] = colour;
                    }
                }
            }
            else if (currentCornerY != CORNERS[cornerIndex][1])
            {
                for (int y = currentCornerY - 1; y <= currentCornerY + 1; y += 2)
                {
                    if (isPositionWithinCubesBorders(y))
                    {
                        colours[currentCornerX][y][currentCornerZ] = colour;
                    }
                }
            }
            else
            {
                for (int z = currentCornerZ - 1; z <= currentCornerZ + 1; z += 2)
                {
                    if (isPositionWithinCubesBorders(z))
                    {
                        colours[currentCornerX][currentCornerY][z] = colour;
                    }
                }
            }
            animationPhase = 0;
            break;
        }
    }
}

void RotatingCube::setRotatingCubeColours(uint32_t colour, int distanceFromCorner)
{
    for (int x = CORNERS[cornerIndex][0] - distanceFromCorner; x <= CORNERS[cornerIndex][0] + distanceFromCorner; x++)
    {
        for (int y = CORNERS[cornerIndex][1] - distanceFromCorner; y <= CORNERS[cornerIndex][1] + distanceFromCorner; y++)
        {
            for (int z = CORNERS[cornerIndex][2] - distanceFromCorner; z <= CORNERS[cornerIndex][2] + distanceFromCorner; z++)
            {
                if (isPositionWithinCubesBorders(x) && isPositionWithinCubesBorders(y) && isPositionWithinCubesBorders(z))
                {
                    colours[x][y][z] = colour;
                }
            }
        }
    }
}

bool RotatingCube::isPositionWithinCubesBorders(int pos)
{
    return pos >= CUBE_AXIS_LOWER_BOUND && pos <= CUBE_AXIS_UPPER_BOUND;
}
