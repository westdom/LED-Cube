#include <avr/sleep.h>
#include "Pattern.h"
#include "Snake.h"

const int POTENTIOMETER_PIN = A1;
const int BUTTON_PIN = 2;
const unsigned int MAX_RUNTIME_DURATION_IN_MINUTES = 60;

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

  checkIfButtonPressed();
  unsigned int delay = checkPotentiometerReading();
  // Checks if enough time has passed to update the pattern again. Using an if like this rather than the "delay()" function is essential as delay() is what's known as a "blocking" delay.
  if (currentTime - lastPatternUpdateStartTime >= delay)
  {
    lastPatternUpdateStartTime = currentTime;
    pattern->update();
  }

  // Checks if the cube has not been turned on/interacted with recently. If it has not, then the board enters "sleep mode" and must be reset to reactivate.
  if (currentTime - lastCubeInteractionStartTime >= MAX_RUNTIME_DURATION_IN_MINUTES * 1000 * 60)
  {
    sleep_enable();
    set_sleep_mode(SLEEP_MODE_PWR_DOWN);
    sleep_cpu();
  }
}

// Strategy design pattern. When we switch pattern via the button, we discard the previous reference of the pattern object from memory, and asign a new pattern dynamically (aka at run time).
void updateSelectedPattern()
{
  static int selectedPattern = 0;

  selectedPattern++;
  if (selectedPattern != 0)
  {
    delete pattern;
  }

  switch (selectedPattern)
  {
  case 0:
    pattern = new Snake("Green Snake", "GREEN", "MAGENTA", true, false);
    break;
  case 1:
    pattern = new Snake("Yellow Snake", "YELLOW", "MAGENTA", true, false);
    break;
  case 2:
    pattern = new Snake("Red Snake", "RED", "MAGENTA", true, false);
    break;
  case 3:
    pattern = new Snake("Rainbow Snake", true, false);
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
