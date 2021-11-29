#include "Snake.h"

#define LED_PIN 3 // the data pin from the Arduino to your string of LEDs
#define DELAY 20 // the pulse width in microseconds (plus the 2 microseconds or so that the digitalWrite command takes)

//Constants
const static int b = 255;
const static int dimensions = 3;
const static int cubeSize = 3;
const static int snakeLength = 3;
const static int cubeAxisUpperBound = cubeSize - 1;
const static int cubeAxisLowerBound = 0;
const static int cubeMapping[cubeSize][cubeSize][cubeSize][dimensions] = {
  {
    { {0, 0, 0}, {1, 0, 0}, {2, 0, 0} },
    { {2, 1, 0}, {1, 1, 0}, {0, 1, 0} },
    { {0, 2, 0}, {1, 2, 0}, {2, 2, 0} }
  },
  {
    { {0, 0, 1}, {1, 0, 1}, {2, 0, 1} },
    { {2, 1, 1}, {1, 1, 1}, {0, 1, 1} },
    { {0, 2, 1}, {1, 2, 1}, {2, 2, 1} }
  },
  {
    { {0, 0, 2}, {1, 0, 2}, {2, 0, 2} },
    { {2, 1, 2}, {1, 1, 2}, {0, 1, 2} },
    { {0, 2, 2}, {1, 2, 2}, {2, 2, 2} }
  }
};
const static int sizeOfLegalMoves = 6;
String legalMoves[sizeOfLegalMoves] = { "LEFT", "RIGHT", "DOWN", "UP", "BACKWARDS", "FORWARDS" };
const String colours[16] =         { "BLACK",   "WHITE",   "RED",     "LIME",    "BLUE",    "YELLOW",  "CYAN",    "MAGENTA", "SILVER",        "GRAY",          "MAROON",    "OLIVE",       "GREEN",     "PURPLE",      "TEAL",        "NAVY" };
const int rgbColourValues[16][3] = { {0, 0, 0}, {b, b, b}, {b, 0, 0}, {0, b, 0}, {0, 0, b}, {b, b, 0}, {0, b, b}, {b, 0, b}, {192, 192, 192}, {128, 128, 128}, {128, 0, 0}, {128, 128, 0}, {0, 128, 0}, {128, 0, 128}, {0, 128, 128}, {0, 0, 128} };

//Variables
int head[dimensions] = {2, 0, 0};
int tail[snakeLength][dimensions] = {
    {2, 0, 0},
    {1, 0, 0},
    {0, 0, 0}
};
int pellet[dimensions] = { 2, 2, 2 };
int positions[snakeLength + 1][dimensions] = {
    {2, 0, 0},
    {1, 0, 0},
    {0, 0, 0},
    {2, 2, 2}
};
int rgbColours[4][3];


Snake::Snake(String snakeColour, String pelletColour, bool pelletOn, bool travelThroughWallsOn) {
  this->snakeColour = snakeColour;
  this->pelletColour = pelletColour;
  this->pelletOn = pelletOn;
  this->travelThroughWallsOn = travelThroughWallsOn;
  setup();
}

void Snake::update() {      
  // put your main code here, to run repeatedly:
  static String backgroundColour = "BLACK";
  static int noOfPositionsToDisplay = pelletOn ? snakeLength + 1 : snakeLength;
  setCubeToPositionsColours(positions, noOfPositionsToDisplay, rgbColours, backgroundColour);
  moveSnakeHeadAndPellet();
  moveSnakeTail();
  setPositionsMatrix();
}

String Snake::getType() {
  return "Snake";
}

void Snake::setup() {
  // put your setup code here, to run once:
  pinMode(LED_PIN, INPUT);
  Serial.begin(9600);
  randomSeed(analogRead(0));
  // Initialises rgbColours
  for(int x = 0; x < 4; x++) {
    for(int y = 0; y < 3; y++) {
      if(x < 3) {
        rgbColours[x][y] = rgbColourValues[findIndexOfString(snakeColour, colours, 16)][y];   
      } else { 
        rgbColours[x][y] = rgbColourValues[findIndexOfString(pelletColour, colours, 16)][y];
      }
    }
  }
}

void Snake::setPositionsMatrix() {
  //Copy tail -> positions
  for(int i = 0; i < snakeLength; i++) {
    for(int j = 0; j < dimensions; j++) {
      positions[i][j] = tail[i][j];
    }
  }
  //Copy pellet -> positions
  for(int i = 0; i < dimensions; i++) {
    positions[snakeLength][i] = pellet[i];
  }
}

void Snake::moveSnakeTail() {
  //Shifts the tail array so that the end of the tail becomes the second to last segment of the tail, the second to last segment of the tail becomes the third to last, etc...
  for(int i = snakeLength - 1; i > 0; i--) {
      for(int j = 0; j < cubeSize; j++) {
          tail[i][j] = tail[i - 1][j];
      }
  }
  //Replaces the start of the tail with the new head
  for(int i = 0; i < cubeSize; i++) {
      tail[0][i] = head[i];
  }
}
    
void Snake::moveSnakeHeadAndPellet() {
  bool moves[] = { false, false, false, false, false, false };
  setPossibleMoves(moves, legalMoves, sizeOfLegalMoves);
  if(pelletOn) {
    if(!moveHeadToPill(moves, legalMoves, sizeOfLegalMoves)) {
      moveHeadInRandomValidDir(moves, legalMoves);
    } else {
      movePelletToRandomUnpopulatedLED();
    }
  } else {
    moveHeadInRandomValidDir(moves, legalMoves);
  }
}
    
//Calculates the possible moves and stores it into the moves boolean array
void Snake::setPossibleMoves(bool moves[], String legalMoves[], int legalMovesSize) {
  for(int i = 0; i < legalMovesSize; i++) {
    String move = legalMoves[i];
    int moveIndex = findIndexOfString(move, legalMoves, legalMovesSize);
    moves[moveIndex] = canMove(move);
  }
}
    
bool Snake::canMove(String move) {
  bool canMove = false;
  int axis = getAxisIndex(move);
  //If we can increment/decrement for the supplied move without breaking through the cubes dimensions
  if(travelThroughWallsOn || (isIncrementMove(move) ? (head[axis] < cubeAxisUpperBound) : (head[axis] > cubeAxisLowerBound))) {
    int originalCoordinate = head[axis];
    head[axis] += getAxisDirection(move);
    if(travelThroughWallsOn) {
      if(head[axis] <  cubeAxisLowerBound) {
        head[axis] = cubeAxisUpperBound;
      } else if(head[axis] > cubeAxisUpperBound) {
        head[axis] = cubeAxisLowerBound;
      }
    }
    //If this position is not a part of the tail, we can move there
    canMove = !matrixContains(tail, snakeLength, head);
    head[axis] = originalCoordinate;
  }
  return canMove;
}
    
//Returns true if the head was adjacent to the pellet and has moved there, else false
bool Snake::moveHeadToPill(bool validMoves[], String legalMoves[], int legalMovesSize) {
  for(int i = 0; i < legalMovesSize; i++) {
    String move = legalMoves[i];
    if(validMoves[i]) {
      int axis = getAxisIndex(move);
      int axisDirection = getAxisDirection(move);
      int originalCoordinate = head[axis];
      head[axis] += axisDirection;
      if(travelThroughWallsOn) {
        if(head[axis] <  cubeAxisLowerBound) {
          head[axis] = cubeAxisUpperBound;
        } else if(head[axis] > cubeAxisUpperBound) {
          head[axis] = cubeAxisLowerBound;
        }
      }
      if(arrayEquals(head, 3, pellet, 3)) {
        return true;
      }
      head[axis] = originalCoordinate;
    }
  }
  return false;
}
    
//Must call setPossibleMoves first. Moves the head of the snake to a random legal adjacent coordinate
void Snake::moveHeadInRandomValidDir(bool validMoves[], String legalMoves[]) {
  bool validMoveFound = false;
  while(!validMoveFound) {
    //Create a random number between 0 - 5
    int randomNumber = random(0, 6);
    //If the random number maps to a possible move, perform the move and exit the loop. Else create new random number
    if(validMoves[randomNumber]) {
      String move = legalMoves[randomNumber];
      int axis = getAxisIndex(move);
      int axisDirection = getAxisDirection(move);
      head[axis] += axisDirection;
      if(travelThroughWallsOn) {
        if(head[axis] <  cubeAxisLowerBound) {
          head[axis] = cubeAxisUpperBound;
        } else if(head[axis] > cubeAxisUpperBound) {
          head[axis] = cubeAxisLowerBound;
        }
      }
      validMoveFound = true;
    }
  }
}
    
//Moves the pellet to a random positiom if the snake ate it
void Snake::movePelletToRandomUnpopulatedLED() {
  bool validPelletPositionFound = false;
  while(!validPelletPositionFound) {
    //Generates random numbers between 0-(cubesize-1)
    int randomXCoordinate = random(0, cubeSize);
    int randomYCoordinate = random(0, cubeSize);
    int randomZCoordinate = random(0, cubeSize);
    int position[] = { randomXCoordinate, randomYCoordinate, randomZCoordinate };
    //If the random position is a part of the snake, generate new number. Else set pellet to random position and exit loop.
    if(!matrixContains(tail, dimensions, position)) {
      validPelletPositionFound = true;
      for(int i = 0; i < dimensions; i++) {
        pellet[i] = position[i];
      }
    }
  }
}
    
int Snake::getAxisIndex(String move) {
  char axis = getAxis(move);
  switch(axis) {
    case 'x':
      return 0;
    case 'y':
      return 1;
    case 'z':
      return 2;
  }
  return -1;
}
    
char Snake::getAxis(String move) {
  if(move.equals("RIGHT") || move.equals("LEFT")) {
    return 'x';
  } else if(move.equals("UP") || move.equals("DOWN")) {
    return 'y';
  } else if(move.equals("FORWARDS") || move.equals("BACKWARDS")) {
    return 'z';
  } else {
    return '-';
  }
}
    
int Snake::getAxisDirection(String move) {
  return isIncrementMove(move) ? 1 : -1;
}

bool Snake::isIncrementMove(String move) {
  return move.equals("UP") || move.equals("RIGHT") || move.equals("FORWARDS");
}

//Sets any coordinates within positions to the supplied onColour, other coordinates are set to the supplied offColour
void Snake::setCubeToPositionsColours(int positions[][3], int noOfPositions, int positionsColours[][3], String offColour) {
  int index = 0;
  int coloursQueue[27][3];
  int positionsLit = 0;
  for(int x = 0; x < cubeSize; x++) {
      for(int y = 0; y < cubeSize; y++) {
          for(int z = 0; z < cubeSize; z++) {
              bool setOnColour = false;
              //This checks if we have already lit up all the positions, if we have then there's no need to waste time checking if the current coordinate matches a position
              if(positionsLit < noOfPositions) {
                  for(int i = 0; i < noOfPositions; i++) {
                      //If the current position matches the current coordinate (we use the cube mapping var here to map coordinates to where they should be within the LED cube), then light up with the onColour. 
                      if(arrayEquals(cubeMapping[x][y][z], 3, positions[i], 3)) {
                          for(int j = 0; j < dimensions; j++) {
                            coloursQueue[index][j] = positionsColours[i][j];
                          }
                          setOnColour = true;
                          positionsLit++;
                          break;
                      }
                  }
              }
              if(!setOnColour) {
                int colourIndex = findIndexOfString(offColour, colours, sizeof(colours));
                for(int j = 0; j < dimensions; j++) {
                  coloursQueue[index][j] = rgbColourValues[colourIndex][j];
                }
              }
              index++;
          }
      }
  }

  for(int i = 0; i < 27; i++) {
    sendColour(coloursQueue[i][0], coloursQueue[i][1], coloursQueue[i][2]);
  }
}
    
//Returns true if arr1 has equal values to arr2, else false
bool Snake::arrayEquals(const int arr1[], int arr1Size, const int arr2[], int arr2Size) {
    if(arr1Size != arr2Size) {
        return false;
    }
    for(int i = 0; i < arr1Size; i++) {
        if(arr1[i] != arr2[i]) {
            return false;
        }
    }
    return true;
}
    
//Returns true if the matrix contains the supplied arr, else false
bool Snake::matrixContains(int matrix[][dimensions], int matrixSize, int arr[dimensions]) {
    for(int i = 0; i < matrixSize; i++) {
        if(arrayEquals(matrix[i], dimensions, arr, dimensions)) {
            return true;
        }
    }
    return false;
}
    
int Snake::findIndexOfString(String string, const String arr[], int arrSize) {
  for(int i = 0; i < arrSize; i++) {
    if(arr[i].equals(string)) {
       return i;
    }
  }
  return -1;
}
    
void Snake::sendColour(byte r, byte g, byte b) {
  sendByte(b); // we send the blue first
  sendByte(g); // then the green
  sendByte(r); // then the blue
}

void Snake::sendByte(byte b) {
  // each byte is sent LSB first and is 8 bits long
  for (int n = 7; n >= 0; n--) {
    // if we have a high bit, set up the digitalWrite first, then make the pin an output
    // this will engage the 10K internal pullup resistor, but in compariosn to our 1K potential divider, this shouldn't affect the logic level
    if (bitRead(b, n)) { 
      digitalWrite(LED_PIN, HIGH);
      pinMode(LED_PIN, OUTPUT);
      delayMicroseconds(DELAY);
      pinMode(LED_PIN, INPUT);
      delayMicroseconds(DELAY);
    }
    // otherwise, if we have a low bit, make the pin an output first and then check it is LOW
    else {
      pinMode(LED_PIN, OUTPUT);
      digitalWrite(LED_PIN, LOW);
      delayMicroseconds(DELAY);
      pinMode(LED_PIN, INPUT);
      delayMicroseconds(DELAY);
    }
  }
}
