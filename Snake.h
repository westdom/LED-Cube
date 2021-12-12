#ifndef Snake_H
#define Snake_H

#include "Pattern.h"

// Interface for the Snake pattern. Implements the Pattern Interface, and defines what methods are required for "Snake.cpp".
class Snake : public Pattern
{
  CRGB leds[27];
  String name;
  uint32_t snakeColour;
  uint32_t pelletColour;
  bool pelletOn;
  bool rainbowSnake;
  const unsigned int DELAY = 150;
  unsigned long previousUpdateStartTime = millis();

public:
  // Fixed colour snake
  Snake(String snakeName, uint32_t snakeColour, uint32_t pelletColour, bool pelletOn);
  // Rainbow snake
  Snake(String snakeName, bool pelletOn);
  virtual void update();
  virtual String getName();

private:
  virtual void setup();
  virtual void setPositionsMatrix();
  virtual void setPositionsColoursMatrix();
  virtual void moveSnakeTail();
  virtual void moveSnakeHeadAndPellet();
  virtual void setPossibleMoves(bool moves[], String legalMoves[], int legalMovesSize);
  virtual bool canMove(String move);
  virtual bool moveHeadToPill(bool validMoves[], String legalMoves[], int legalMovesSize);
  virtual void moveHeadInRandomValidDir(bool validMoves[], String legalMoves[]);
  virtual void movePelletToRandomUnpopulatedLED();
  virtual int getAxisIndex(String move);
  virtual char getAxis(String move);
  virtual int getAxisDirection(String move);
  virtual bool isIncrementMove(String move);
};

#endif
