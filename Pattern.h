#ifndef Pattern_H
#define Pattern_H

#include <Arduino.h>
#include <FastLED.h>

// Abstract class that forces classes which implement it to have the "update" and "getName" functions. They also inherit the below generic utility methods.
class Pattern
{
public:
    const static int MAX_BRIGHTNESS = 40;
    const static int LED_PIN = 4;
    const static int DIMENSIONS = 3;
    const static int CUBE_SIZE = 3;
    const static int CUBE_AXIS_UPPER_BOUND = CUBE_SIZE - 1;
    const static int CUBE_AXIS_LOWER_BOUND = 0;

    // Virtual forces classes that implement this interface to override these functions.
    virtual ~Pattern() = default;
    virtual void update();
    virtual String getName();

    /* GENERIC UTILITY METHODS */

    boolean arrayEquals(const int arr1[], const int ARR_1_SIZE, const int arr2[], const int ARR_2_SIZE)
    {
        if (ARR_1_SIZE != ARR_2_SIZE)
        {
            return false;
        }
        for (int i = 0; i < ARR_1_SIZE; i++)
        {
            if (arr1[i] != arr2[i])
            {
                return false;
            }
        }
        return true;
    }

    boolean matrixContains(int matrix[][3], const int MATRIX_SIZE, int arr[3])
    {
        for (int i = 0; i < MATRIX_SIZE; i++)
        {
            if (arrayEquals(matrix[i], 3, arr, 3))
            {
                return true;
            }
        }
        return false;
    }

    int findIndexOfString(const String STRING, const String arr[], const int ARR_SIZE)
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

    uint32_t getNextRainbowColour()
    {
        static const uint8_t lights[360] = {
            0, 0, 0, 0, 0, 1, 1, 2, 2, 3, 4, 5, 6, 7, 8, 9, 11, 12, 13, 15, 17, 18, 20, 22, 24, 26, 28, 30, 32, 35, 37, 39, 42, 44, 47, 49, 52, 55, 58, 60,
            63, 66, 69, 72, 75, 78, 81, 85, 88, 91, 94, 97, 101, 104, 107, 111, 114, 117, 121, 124, 127, 131, 134, 137, 141, 144, 147, 150, 154, 157, 160, 163,
            167, 170, 173, 176, 179, 182, 185, 188, 191, 194, 197, 200, 202, 205, 208, 210, 213, 215, 217, 220, 222, 224, 226, 229, 231, 232, 234, 236, 238, 239, 241, 242,
            244, 245, 246, 248, 249, 250, 251, 251, 252, 253, 253, 254, 254, 255, 255, 255, 255, 255, 255, 255, 254, 254, 253, 253, 252, 251, 251, 250, 249, 248, 246, 245,
            244, 242, 241, 239, 238, 236, 234, 232, 231, 229, 226, 224, 222, 220, 217, 215, 213, 210, 208, 205, 202, 200, 197, 194, 191, 188, 185, 182, 179, 176, 173, 170,
            167, 163, 160, 157, 154, 150, 147, 144, 141, 137, 134, 131, 127, 124, 121, 117, 114, 111, 107, 104, 101, 97, 94, 91, 88, 85, 81, 78, 75, 72, 69, 66,
            63, 60, 58, 55, 52, 49, 47, 44, 42, 39, 37, 35, 32, 30, 28, 26, 24, 22, 20, 18, 17, 15, 13, 12, 11, 9, 8, 7, 6, 5, 4, 3, 2, 2, 1, 1, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
        static int angle = 0;
        angle++;
        if (angle >= 360)
        {
            angle = 0;
        }
        int red = lights[(angle + 120) % 360];
        int green = lights[angle];
        int blue = lights[(angle + 240) % 360];
        uint32_t hexColour = ((red & 0xFFL) << 16L) + ((green & 0xFFL) << 8L) + (blue & 0xFFL);
        return hexColour;
    }

    void sendColour(CRGB leds[], const int LED_INDEX, uint32_t hexColour)
    {
        int red = ((hexColour >> 16) & 0xFF);
        int green = ((hexColour >> 8) & 0xFF);
        int blue = ((hexColour)&0xFF);
        // Swapping R and G as FASTLED seems to import hexs as GGRRBB
        uint32_t swappedColour = ((green & 0xFFL) << 16L) + ((red & 0xFFL) << 8L) + (blue & 0xFFL);
        leds[LED_INDEX] = swappedColour;
    }

    void sendColour(CRGB leds[], const int LED_INDEX, int rgb[])
    {
        leds[LED_INDEX].setRGB(rgb[0], rgb[1], rgb[2]);
    }

    void sendColours(CRGB leds[], uint32_t positionsColours[3][3][3])
    {
        static const int CUBE_MAPPINGS[3][3][3][3] = {
            {{{2, 0, 0}, {1, 0, 0}, {0, 0, 0}},
             {{0, 0, 1}, {1, 0, 1}, {2, 0, 1}},
             {{2, 0, 2}, {1, 0, 2}, {0, 0, 2}}},
            {{{2, 1, 2}, {1, 1, 2}, {0, 1, 2}},
             {{0, 1, 1}, {1, 1, 1}, {2, 1, 1}},
             {{2, 1, 0}, {1, 1, 0}, {0, 1, 0}}},
            {{{2, 2, 0}, {1, 2, 0}, {0, 2, 0}},
             {{0, 2, 1}, {1, 2, 1}, {2, 2, 1}},
             {{2, 2, 2}, {1, 2, 2}, {0, 2, 2}}}};
        int index = 0;
        for (int x = 0; x < CUBE_SIZE; x++)
        {
            for (int y = 0; y < CUBE_SIZE; y++)
            {
                for (int z = 0; z < CUBE_SIZE; z++)
                {
                    const uint32_t COLOUR = positionsColours[CUBE_MAPPINGS[x][y][z][0]][CUBE_MAPPINGS[x][y][z][1]][CUBE_MAPPINGS[x][y][z][2]];
                    if (COLOUR)
                    {
                        sendColour(leds, index, COLOUR);
                    }
                    else
                    {
                        sendColour(leds, index, CRGB::Black);
                    }
                    index++;
                }
            }
        }
        FastLED.show();
    }

    void sendPositionsColours(CRGB leds[], int positions[][3], const int NUMBER_OF_POSITIONS, uint32_t positionsColours[], const long OFF_COLOUR)
    {
        static const int CUBE_MAPPINGS[3][3][3][3] = {
            {{{2, 0, 0}, {1, 0, 0}, {0, 0, 0}},
             {{0, 0, 1}, {1, 0, 1}, {2, 0, 1}},
             {{2, 0, 2}, {1, 0, 2}, {0, 0, 2}}},
            {{{2, 1, 2}, {1, 1, 2}, {0, 1, 2}},
             {{0, 1, 1}, {1, 1, 1}, {2, 1, 1}},
             {{2, 1, 0}, {1, 1, 0}, {0, 1, 0}}},
            {{{2, 2, 0}, {1, 2, 0}, {0, 2, 0}},
             {{0, 2, 1}, {1, 2, 1}, {2, 2, 1}},
             {{2, 2, 2}, {1, 2, 2}, {0, 2, 2}}}};
        int index = 0;
        int positionsLit = 0;
        for (int x = 0; x < CUBE_SIZE; x++)
        {
            for (int y = 0; y < CUBE_SIZE; y++)
            {
                for (int z = 0; z < CUBE_SIZE; z++)
                {
                    bool setOnColour = false;
                    // This checks if we have already lit up all the positions, if we have then there's no need to waste time checking if the current coordinate matches a position
                    if (positionsLit < NUMBER_OF_POSITIONS)
                    {
                        for (int i = 0; i < NUMBER_OF_POSITIONS; i++)
                        {
                            // If the current position matches the current coordinate (we use the cube mapping var here to map coordinates to where they should be within the LED cube), then light up with the onColour.
                            if (arrayEquals(CUBE_MAPPINGS[x][y][z], 3, positions[i], 3))
                            {
                                sendColour(leds, index, positionsColours[i]);
                                setOnColour = true;
                                positionsLit++;
                                break;
                            }
                        }
                    }
                    if (!setOnColour)
                    {
                        sendColour(leds, index, OFF_COLOUR);
                    }
                    index++;
                }
            }
        }
        FastLED.show();
    }
};

#endif
