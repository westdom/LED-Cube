#include "Pattern.h"
#include "Snake.h"

unsigned long start_time;
unsigned long current_time;
unsigned long lastDebounceTime = 0;  // the last time the output pin was toggled
unsigned long debounceDelay = 100;    // the debounce time; increase if the output flickers
int lastButtonState = LOW;   // the previous reading from the input pin
int potentiometerPin = A1; // the analog input pin attach to
int buttonPin = 2;     // the number of the pushbutton pin
int buttonState = 0;
int noOfLEDCubePatterns = 1;
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
        if(!pattern->getType().equals("Snake")) {
          pattern = new Snake("GREEN", "MAGENTA", true, false);
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
