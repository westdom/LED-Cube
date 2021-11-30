#include "Pattern.h"
#include "Snake.h"

const int potentiometerPin = A1;
const int buttonPin = 2;

unsigned long start_time;
unsigned long current_time;
unsigned long lastDebounceTime = 0; // the last time the buttons pin was toggled.
unsigned long debounceDelay = 100;  // the debounce time; increase/decrease accordingly if button presses seem dodgey.
int lastButtonState = LOW;
int buttonState = 0;

Pattern *pattern;

void setup()
{
  pinMode(buttonPin, INPUT);
  updateSelectedPattern();
  current_time = millis();
  start_time = current_time;
}

void loop()
{
  current_time = millis();
  checkIfButtonPressed();
  unsigned int delay = analogRead(potentiometerPin);
  // Checks if enough time has passed to update the pattern again. Using an if like this rather than the "delay()" function is essential as this method is what's known as a "non-blocking" delay.
  if (current_time - start_time >= delay)
  {
    start_time = current_time;
    pattern->update();
  }
}

void checkIfButtonPressed()
{
  int reading = digitalRead(buttonPin);
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
        updateSelectedPattern();
      }
    }
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
