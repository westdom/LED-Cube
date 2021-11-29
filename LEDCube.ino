#include "Pattern.h"
#include "Snake.h"

const int potentiometerPin = A1;
const int buttonPin = 2;
const int noOfLEDCubePatterns = 3;

unsigned long start_time;
unsigned long current_time;
unsigned long lastDebounceTime = 0;  // the last time the buttons pin was toggled.
unsigned long debounceDelay = 100;   // the debounce time; increase/decrease accordingly if button presses seem dodgey.
int lastButtonState = LOW;
int buttonState = 0;
int selectedPattern = 0;

Pattern *pattern;

void setup() {
  pinMode(buttonPin, INPUT);
  current_time = millis();
  start_time = current_time;
}

void loop() {
  current_time = millis();
  checkButton();
  int delay = analogRead(potentiometerPin);
  // Rather than using the delay() function here, a check for if enough time has expired allows us to continue running code (essential for continously checking button presses/potentiometer changes).
  // This is because delay() is whats known as a "blocking" delay.
  if(current_time - start_time >= delay) {
    start_time = current_time;
    // Strategy design pattern. When we switch pattern via the button, we discard the previous reference of the pattern object from memory, and asign a new pattern dynamically (aka at run time).
    switch(selectedPattern) {
      case 0:
        if(!pattern->getName().equals("Green Snake")) {
          delete pattern;
          pattern = new Snake("Green Snake", "GREEN", "MAGENTA", true, false);
        }
        break;
      case 1:
        if(!pattern->getName().equals("Yellow Snake")) {
          delete pattern;
          pattern = new Snake("Yellow Snake", "YELLOW", "MAGENTA", true, false);
        }
        break;
      case 2:
        if(!pattern->getName().equals("Red Snake")) {
          delete pattern;
          pattern = new Snake("Red Snake", "RED", "MAGENTA", true, false);
        }
        break;
    }
    // Because all the patterns implement the "Pattern.h" interface, this update function runs the update function for the currently selected pattern.
    pattern->update();
  }
}

void checkButton() {
  int reading = digitalRead(buttonPin);
  if(reading != lastButtonState) {
    lastDebounceTime = millis();
  }
  // Debouncing should mean button presses are more accurate in changing the selected pattern.
  if ((millis() - lastDebounceTime) > debounceDelay) {
    if (reading != buttonState) {
      buttonState = reading;
      if(buttonState == HIGH) {
        cycleToNextPattern();
      }
    }
  }
}

void cycleToNextPattern() {
  selectedPattern++;
  if(selectedPattern >= noOfLEDCubePatterns) {
    selectedPattern = 0;
  }
}
