#ifndef Snake_H
#define Snake_H

#include "Pattern.h"

// Interface for the Snake pattern. Implements the Pattern Interface, and defines what methods are required for "Snake.cpp".
class Snake : public Pattern
{
  CRGB LEDS[27];
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
  void setup();
  void setPositionsMatrix();
  void moveSnakeTail();
  void moveSnakeHeadAndPellet();
  void shiftColours();
  void setPossibleMoves(bool moves[], String legalMoves[], int legalMovesSize);
  bool canMove(String move);
  bool moveHeadToPill(bool validMoves[], String legalMoves[], int legalMovesSize);
  void moveHeadInRandomValidDir(bool validMoves[], String legalMoves[]);
  void movePelletToRandomUnpopulatedLED();
  int getAxisIndex(String move);
  char getAxis(String move);
  int getAxisDirection(String move);
  bool isIncrementMove(String move);
  void setCubeToPositionsColours(int positions[][3], int noOfPositions, int positionsColours[][3], String offColour);
  bool arrayEquals(const int arr1[], int arr1Size, const int arr2[], int arr2Size);
  bool matrixContains(int matrix[][3], int matrixSize, int arr[3]);
  int findIndexOfString(String string, const String arr[], int arrSize);
};

#endif
