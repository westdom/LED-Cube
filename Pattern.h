#ifndef Pattern_H
#define Pattern_H

#include <Arduino.h>
#include <FastLED.h>

// Abstract class that forces classes which implement it to have the "update" and "getName" functions. They also inherit the below generic utility methods.
class Pattern
{
    // Public means if you have a reference to an object of type Pattern, you can invoke the below functions on that object.
public:
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

    void shiftColour(int rgbColour[], const int MAX_BRIGHTNESS)
    {
        const int INCREMENT_BY = 10;

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
        if (colourIndex == (MAX_BRIGHTNESS / INCREMENT_BY))
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
        rgbColour[decColour] -= INCREMENT_BY;
        rgbColour[incColour] += INCREMENT_BY;
    }

    void sendColour(CRGB leds[], const int LED_INDEX, int rgbColour[])
    {
        leds[LED_INDEX].red = rgbColour[0];
        leds[LED_INDEX].green = rgbColour[1];
        leds[LED_INDEX].blue = rgbColour[2];
    }
};

#endif