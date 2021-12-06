#include <avr/sleep.h>
#include "Pattern.h"
#include "Snake.h"
#include "SolidColour.h"
#include "RotatingPlaneColourCycle.h"

const int POTENTIOMETER_PIN = A1;
const int BUTTON_PIN = 2;
const unsigned long MAX_RUNTIME_DURATION = 1000L * 60L * 30L;

unsigned long lastCubeInteractionStartTime = millis();

Pattern *pattern;

void setup()
{
  Serial.begin(9600);
  pinMode(BUTTON_PIN, INPUT);
  updateSelectedPattern();
}

void loop()
{
  static unsigned long currentTime = millis();
  static unsigned long lastPatternUpdateStartTime = currentTime;

  currentTime = millis();
  // checkIfButtonPressed();
  unsigned int delay = 100 * pattern->getDelayMultiplier();
  // Checks if enough time has passed to update the pattern again. Using an if like this rather than the "delay()" function is essential as delay() is what's known as a "blocking" delay.
  if (currentTime - lastPatternUpdateStartTime >= delay)
  {
    lastPatternUpdateStartTime = currentTime;
    pattern->update();
  }

  // Checks if the cube has not been turned on/interacted with recently. If it has not, then the board enters "sleep mode" and must be reset to reactivate.
  if (currentTime - lastCubeInteractionStartTime >= MAX_RUNTIME_DURATION)
  {
    delete pattern;
    pattern = new SolidColour("Black Cube", 0x000000);
    pattern->update();
    sleep_enable();
    set_sleep_mode(SLEEP_MODE_PWR_DOWN);
    sleep_cpu();
  }
}

// Strategy design pattern. When we switch pattern via the button, we discard the previous reference of the pattern object from memory, and asign a new pattern dynamically (aka at run time).
void updateSelectedPattern()
{
  static int selectedPattern = 4;

  // selectedPattern++;
  if (selectedPattern != 0)
  {
    // delete pattern;
  }

  switch (selectedPattern)
  {
  case 0:
    pattern = new SolidColour("Red Cube", 0xFF0000);
    break;
  case 1:
    pattern = new SolidColour("Rainbow Cube");
    break;
  case 2:
    pattern = new Snake("Green Snake", 0x008000, 0xFF00FF, true, false);
    break;
  case 3:
    pattern = new Snake("Rainbow Snake", true, false);
    break;
  case 4:
    pattern = new RotatingPlaneColourCycle("Rotating Plane Colour Cycle");
    break;
  default:
    selectedPattern = -1;
    updateSelectedPattern();
    break;
  }
}

unsigned int checkPotentiometerReading()
{
  static unsigned int lastPotentiometerReading = analogRead(POTENTIOMETER_PIN);

  unsigned int potontiometerReading = analogRead(POTENTIOMETER_PIN);
  if (potontiometerReading != lastPotentiometerReading)
  {
    lastCubeInteractionStartTime = millis();
  }
  lastPotentiometerReading = potontiometerReading;
  return potontiometerReading;
}

void checkIfButtonPressed()
{
  static unsigned long lastDebounceTime = 0; // the last time the buttons pin was toggled.
  static unsigned long debounceDelay = 100;  // the debounce time; increase/decrease accordingly if button presses seem dodgey.
  static int lastButtonState = LOW;
  static int buttonState = 0;

  int reading = digitalRead(BUTTON_PIN);
  if (reading != lastButtonState)
  {
    lastDebounceTime = millis();
  }
  // Debouncing should mean button presses are more accurate in changing the selected pattern.
  if ((millis() - lastDebounceTime) > debounceDelay)
  {
    if (reading != buttonState)
    {
      buttonState = reading;
      if (buttonState == HIGH)
      {
        lastCubeInteractionStartTime = millis();
        updateSelectedPattern();
      }
    }
  }
}
