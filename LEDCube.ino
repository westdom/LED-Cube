#include "Pattern.h"
#include "Snake.h"

const int potentiometerPin = A1;
const int buttonPin = 2;
const int noOfLEDCubePatterns = 3;

unsigned long start_time;
unsigned long current_time;
unsigned long lastDebounceTime = 0;  // the last time the buttons pin was toggled
unsigned long debounceDelay = 100;   // the debounce time; increase if the output flickers
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
  checkButton();
  current_time = millis();
  int delay = analogRead(potentiometerPin);
  if(current_time - start_time >= delay) {
    start_time = current_time;
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
    pattern->update();
  }
}

void checkButton() {
  int reading = digitalRead(buttonPin);
  if(reading != lastButtonState) {
    lastDebounceTime = millis();
  }
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
