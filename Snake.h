#ifndef Snake_H
#define Snake_H

#include "Pattern.h"

// Interface for the Snake pattern. Implements the Pattern Interface, and defines what methods are required for "Snake.cpp".
class Snake : public Pattern
{
  CRGB leds[27];
  String name;
  String snakeColour;
  String pelletColour;
  bool pelletOn;
  bool travelThroughWallsOn;
  bool rainbowSnake;

public:
  // Fixed colour snake
  Snake(String snakeName, String snakeColour, String pelletColour, bool pelletOn, bool travelThroughWallsOn);
  // Rainbow snake
  Snake(String snakeName, bool pelletOn, bool travelThroughWallsOn);
  virtual void update();
  virtual String getName();

private:
  virtual void setup();
  virtual void setPositionsMatrix();
  virtual void moveSnakeTail();
  virtual void moveSnakeHeadAndPellet();
  virtual void shiftColours();
  virtual void setPossibleMoves(bool moves[], String legalMoves[], int legalMovesSize);
  virtual bool canMove(String move);
  virtual bool moveHeadToPill(bool validMoves[], String legalMoves[], int legalMovesSize);
  virtual void moveHeadInRandomValidDir(bool validMoves[], String legalMoves[]);
  virtual void movePelletToRandomUnpopulatedLED();
  virtual int getAxisIndex(String move);
  virtual char getAxis(String move);
  virtual int getAxisDirection(String move);
  virtual bool isIncrementMove(String move);
  virtual void setCubeToPositionsColours(int positions[][3], int noOfPositions, int positionsColours[][3], String offColour);
  virtual bool arrayEquals(const int arr1[], int arr1Size, const int arr2[], int arr2Size);
  virtual bool matrixContains(int matrix[][3], int matrixSize, int arr[3]);
  virtual int findIndexOfString(String string, const String arr[], int arrSize);
};

#endif
