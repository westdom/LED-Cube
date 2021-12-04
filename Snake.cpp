#include "Snake.h"

#define LED_PIN 3 // the data pin from the Arduino to your string of LEDs
#define DELAY 20  // the pulse width in microseconds (plus the 2 microseconds or so that the digitalWrite command takes)

// Constants
const static int MAX_BRIGHTNESS = 50;
const static int DIMENSIONS = 3;
const static int CUBE_SIZE = 3;
const static int SNAKE_LENGTH = 3;
const static int CUBE_AXIS_UPPER_BOUND = CUBE_SIZE - 1;
const static int CUBE_AXIS_LOWER_BOUND = 0;
const static int CUBE_MAPPINGS[CUBE_SIZE][CUBE_SIZE][CUBE_SIZE][DIMENSIONS] = {
    {{{0, 0, 0}, {1, 0, 0}, {2, 0, 0}},
     {{2, 1, 0}, {1, 1, 0}, {0, 1, 0}},
     {{0, 2, 0}, {1, 2, 0}, {2, 2, 0}}},
    {{{0, 0, 1}, {1, 0, 1}, {2, 0, 1}},
     {{2, 1, 1}, {1, 1, 1}, {0, 1, 1}},
     {{0, 2, 1}, {1, 2, 1}, {2, 2, 1}}},
    {{{0, 0, 2}, {1, 0, 2}, {2, 0, 2}},
     {{2, 1, 2}, {1, 1, 2}, {0, 1, 2}},
     {{0, 2, 2}, {1, 2, 2}, {2, 2, 2}}}};
const String COLOURS[8] = {"BLACK", "WHITE", "RED", "LIME", "BLUE", "YELLOW", "CYAN", "MAGENTA"};
const int RGB_COLOUR_VALUES[8][3] = {{0, 0, 0}, {MAX_BRIGHTNESS, MAX_BRIGHTNESS, MAX_BRIGHTNESS}, {MAX_BRIGHTNESS, 0, 0}, {0, MAX_BRIGHTNESS, 0}, {0, 0, MAX_BRIGHTNESS}, {MAX_BRIGHTNESS, MAX_BRIGHTNESS, 0}, {0, MAX_BRIGHTNESS, MAX_BRIGHTNESS}, {MAX_BRIGHTNESS, 0, MAX_BRIGHTNESS}};
const static int SIZE_OF_LEGAL_MOVES = 6;
String LEGAL_MOVES[SIZE_OF_LEGAL_MOVES] = {"LEFT", "RIGHT", "DOWN", "UP", "BACKWARDS", "FORWARDS"};

// Variables
int head[DIMENSIONS] = {2, 0, 0};
int tail[SNAKE_LENGTH][DIMENSIONS] = {
    {2, 0, 0},
    {1, 0, 0},
    {0, 0, 0}};
int pellet[DIMENSIONS] = {2, 2, 2};
int positions[SNAKE_LENGTH + 1][DIMENSIONS] = {
    {2, 0, 0},
    {1, 0, 0},
    {0, 0, 0},
    {2, 2, 2}};
int ledOnColours[4][3];

Snake::Snake(const String SNAKE_NAME, const String SNAKE_COLOUR, const String PELLET_COLOUR, const bool PELLET_ON, const bool TRAVEL_THROUGH_WALLS_ON)
{
  this->name = SNAKE_NAME;
  this->snakeColour = SNAKE_COLOUR;
  this->pelletColour = PELLET_COLOUR;
  this->pelletOn = PELLET_ON;
  this->travelThroughWallsOn = TRAVEL_THROUGH_WALLS_ON;
  this->rainbowSnake = false;
  setup();
}

Snake::Snake(const String SNAKE_NAME, const bool PELLET_ON, const bool TRAVEL_THROUGH_WALLS_ON)
{
  this->name = SNAKE_NAME;
  this->snakeColour = "GREEN";
  this->pelletColour = "MAGENTA";
  this->pelletOn = PELLET_ON;
  this->travelThroughWallsOn = TRAVEL_THROUGH_WALLS_ON;
  this->rainbowSnake = true;
  setup();
}

void Snake::update()
{
  // put your main code here, to run repeatedly:
  const static String BACKGROUND_COLOUR = "BLACK";
  const static int NUMBER_OF_LEDS_TO_DISPLAY = pelletOn ? SNAKE_LENGTH + 1 : SNAKE_LENGTH;
  setCubeToPositionsColours(positions, NUMBER_OF_LEDS_TO_DISPLAY, ledOnColours, BACKGROUND_COLOUR);
  moveSnakeHeadAndPellet();
  moveSnakeTail();
  setPositionsMatrix();
  if (rainbowSnake)
  {
    shiftColours();
  }
}

String Snake::getName()
{
  return name;
}

void Snake::setup()
{
  // put your setup code here, to run once:
  FastLED.addLeds<NEOPIXEL, LED_PIN>(leds, 27);
  randomSeed(analogRead(0));
  for (int x = 0; x < 4; x++)
  {
    for (int y = 0; y < 3; y++)
    {
      if (x < 3)
      {
        ledOnColours[x][y] = RGB_COLOUR_VALUES[findIndexOfString(snakeColour, COLOURS, 16)][y];
      }
      else
      {
        ledOnColours[x][y] = RGB_COLOUR_VALUES[findIndexOfString(pelletColour, COLOURS, 16)][y];
      }
    }
  }
}

void Snake::setPositionsMatrix()
{
  // Copy tail -> positions
  for (int i = 0; i < SNAKE_LENGTH; i++)
  {
    for (int j = 0; j < DIMENSIONS; j++)
    {
      positions[i][j] = tail[i][j];
    }
  }
  // Copy pellet -> positions
  for (int i = 0; i < DIMENSIONS; i++)
  {
    positions[SNAKE_LENGTH][i] = pellet[i];
  }
}

void Snake::moveSnakeTail()
{
  // Shifts the tail array so that the end of the tail becomes the second to last segment of the tail, the second to last segment of the tail becomes the third to last, etc...
  for (int i = SNAKE_LENGTH - 1; i > 0; i--)
  {
    for (int j = 0; j < CUBE_SIZE; j++)
    {
      tail[i][j] = tail[i - 1][j];
    }
  }
  // Replaces the start of the tail with the new head
  for (int i = 0; i < CUBE_SIZE; i++)
  {
    tail[0][i] = head[i];
  }
}

void Snake::moveSnakeHeadAndPellet()
{
  bool moves[] = {false, false, false, false, false, false};
  setPossibleMoves(moves, LEGAL_MOVES, SIZE_OF_LEGAL_MOVES);
  if (pelletOn)
  {
    if (!moveHeadToPill(moves, LEGAL_MOVES, SIZE_OF_LEGAL_MOVES))
    {
      moveHeadInRandomValidDir(moves, LEGAL_MOVES);
    }
    else
    {
      movePelletToRandomUnpopulatedLED();
    }
  }
  else
  {
    moveHeadInRandomValidDir(moves, LEGAL_MOVES);
  }
}

void Snake::shiftColours()
{
  static unsigned int rgbColour[3];
  static int decColour = 0;
  static int incColour = 2;
  static int colourIndex = 0;

  if (colourIndex == 0)
  {
    // Reset rgbColours
    rgbColour[0] = ledOnColours[0][0];
    rgbColour[1] = ledOnColours[0][1];
    rgbColour[2] = ledOnColours[0][2];
  }

  colourIndex++;
  if (colourIndex == MAX_BRIGHTNESS)
  {
    colourIndex = 0;

    // Shift dec & inc colours to next rgb byte
    decColour++;
    incColour--;
    if (decColour > 2 || incColour < 0)
    {
      decColour = 0;
      incColour = 2;
    }
  }
  // Increment/decrement respective bytes of rgbColour
  const static int INCREMENT_BY = 10;
  rgbColour[decColour] -= INCREMENT_BY;
  rgbColour[incColour] += INCREMENT_BY;

  // Copy new rgbcolour to the snakes colours
  for (int i = 0; i < SNAKE_LENGTH; i++)
  {
    ledOnColours[0][i] = rgbColour[i];
    ledOnColours[1][i] = rgbColour[i];
    ledOnColours[2][i] = rgbColour[i];
  }
}

// Calculates the possible moves and stores it into the moves boolean array
void Snake::setPossibleMoves(bool moves[], String legalMoves[], const int LEGAL_MOVES_SIZE)
{
  for (int i = 0; i < LEGAL_MOVES_SIZE; i++)
  {
    String move = legalMoves[i];
    int moveIndex = findIndexOfString(move, legalMoves, LEGAL_MOVES_SIZE);
    moves[moveIndex] = canMove(move);
  }
}

bool Snake::canMove(const String MOVE)
{
  bool canMove = false;
  const int AXIS = getAxisIndex(MOVE);
  // If we can increment/decrement for the supplied move without breaking through the cubes dimensions
  if (travelThroughWallsOn || (isIncrementMove(MOVE) ? (head[AXIS] < CUBE_AXIS_UPPER_BOUND) : (head[AXIS] > CUBE_AXIS_LOWER_BOUND)))
  {
    int originalCoordinate = head[AXIS];
    head[AXIS] += getAxisDirection(MOVE);
    if (travelThroughWallsOn)
    {
      if (head[AXIS] < CUBE_AXIS_LOWER_BOUND)
      {
        head[AXIS] = CUBE_AXIS_UPPER_BOUND;
      }
      else if (head[AXIS] > CUBE_AXIS_UPPER_BOUND)
      {
        head[AXIS] = CUBE_AXIS_LOWER_BOUND;
      }
    }
    // If this position is not a part of the tail, we can move there
    canMove = !matrixContains(tail, SNAKE_LENGTH, head);
    head[AXIS] = originalCoordinate;
  }
  return canMove;
}

// Returns true if the head was adjacent to the pellet and has moved there, else false
bool Snake::moveHeadToPill(bool validMoves[], String legalMoves[], const int LEGAL_MOVES_SIZE)
{
  for (int i = 0; i < LEGAL_MOVES_SIZE; i++)
  {
    const String MOVE = legalMoves[i];
    if (validMoves[i])
    {
      const int AXIS = getAxisIndex(MOVE);
      const int AXIS_DIRECTION = getAxisDirection(MOVE);
      const int ORIGINAL_COORDINATE = head[AXIS];
      head[AXIS] += AXIS_DIRECTION;
      if (travelThroughWallsOn)
      {
        if (head[AXIS] < CUBE_AXIS_LOWER_BOUND)
        {
          head[AXIS] = CUBE_AXIS_UPPER_BOUND;
        }
        else if (head[AXIS] > CUBE_AXIS_UPPER_BOUND)
        {
          head[AXIS] = CUBE_AXIS_LOWER_BOUND;
        }
      }
      if (arrayEquals(head, 3, pellet, 3))
      {
        return true;
      }
      head[AXIS] = ORIGINAL_COORDINATE;
    }
  }
  return false;
}

// Must call setPossibleMoves first. Moves the head of the snake to a random legal adjacent coordinate
void Snake::moveHeadInRandomValidDir(bool validMoves[], String legalMoves[])
{
  bool validMoveFound = false;
  while (!validMoveFound)
  {
    // Create a random number between 0 - 5
    const int RANDOM_NUMBER = random(0, 6);
    // If the random number maps to a possible move, perform the move and exit the loop. Else create new random number
    if (validMoves[RANDOM_NUMBER])
    {
      const String MOVE = legalMoves[RANDOM_NUMBER];
      const int AXIS = getAxisIndex(MOVE);
      const int AXIS_DIRECTION = getAxisDirection(MOVE);
      head[AXIS] += AXIS_DIRECTION;
      if (travelThroughWallsOn)
      {
        if (head[AXIS] < CUBE_AXIS_LOWER_BOUND)
        {
          head[AXIS] = CUBE_AXIS_UPPER_BOUND;
        }
        else if (head[AXIS] > CUBE_AXIS_UPPER_BOUND)
        {
          head[AXIS] = CUBE_AXIS_LOWER_BOUND;
        }
      }
      validMoveFound = true;
    }
  }
}

// Moves the pellet to a random positiom if the snake ate it
void Snake::movePelletToRandomUnpopulatedLED()
{
  bool validPelletPositionFound = false;
  while (!validPelletPositionFound)
  {
    // Generates random numbers between 0-(cubesize-1)
    const int RANDOM_X_COORDINATE = random(0, CUBE_SIZE);
    const int RANDOM_Y_COORDINATE = random(0, CUBE_SIZE);
    const int RANDOM_Z_COORDINATE = random(0, CUBE_SIZE);
    int position[] = {RANDOM_X_COORDINATE, RANDOM_Y_COORDINATE, RANDOM_Z_COORDINATE};
    // If the random position is a part of the snake, generate new number. Else set pellet to random position and exit loop.
    if (!matrixContains(tail, DIMENSIONS, position))
    {
      validPelletPositionFound = true;
      for (int i = 0; i < DIMENSIONS; i++)
      {
        pellet[i] = position[i];
      }
    }
  }
}

int Snake::getAxisIndex(const String MOVE)
{
  const char AXIS = getAxis(MOVE);
  switch (AXIS)
  {
  case 'x':
    return 0;
  case 'y':
    return 1;
  case 'z':
    return 2;
  }
  return -1;
}

char Snake::getAxis(const String MOVE)
{
  if (MOVE.equals("RIGHT") || MOVE.equals("LEFT"))
  {
    return 'x';
  }
  else if (MOVE.equals("UP") || MOVE.equals("DOWN"))
  {
    return 'y';
  }
  else if (MOVE.equals("FORWARDS") || MOVE.equals("BACKWARDS"))
  {
    return 'z';
  }
  else
  {
    return '-';
  }
}

int Snake::getAxisDirection(const String MOVE)
{
  return isIncrementMove(MOVE) ? 1 : -1;
}

bool Snake::isIncrementMove(const String MOVE)
{
  return MOVE.equals("UP") || MOVE.equals("RIGHT") || MOVE.equals("FORWARDS");
}

// Sets any coordinates within positions to the supplied onColour, other coordinates are set to the supplied offColour
void Snake::setCubeToPositionsColours(int positions[][3], const int NUMBER_OF_POSITIONS, int positionsColours[][3], const String OFF_COLOUR)
{
  int index = 0;
  int positionsLit = 0;
  for (int x = 0; x < CUBE_SIZE; x++)
  {
    for (int y = 0; y < CUBE_SIZE; y++)
    {
      for (int z = 0; z < CUBE_SIZE; z++)
      {
        bool setOnColour = false;
        // This checks if we have already lit up all the positions, if we have then there's no need to waste time checking if the current coordinate matches a position
        if (positionsLit < NUMBER_OF_POSITIONS)
        {
          for (int i = 0; i < NUMBER_OF_POSITIONS; i++)
          {
            // If the current position matches the current coordinate (we use the cube mapping var here to map coordinates to where they should be within the LED cube), then light up with the onColour.
            if (arrayEquals(CUBE_MAPPINGS[x][y][z], 3, positions[i], 3))
            {
              leds[index].red = positionsColours[i][0];
              leds[index].green = positionsColours[i][1];
              leds[index].blue = positionsColours[i][2];
              setOnColour = true;
              positionsLit++;
              break;
            }
          }
        }
        if (!setOnColour)
        {
          const int COLOUR_INDEX = findIndexOfString(OFF_COLOUR, COLOURS, sizeof(COLOURS));
          leds[index].red = RGB_COLOUR_VALUES[COLOUR_INDEX][0];
          leds[index].green = RGB_COLOUR_VALUES[COLOUR_INDEX][1];
          leds[index].blue = RGB_COLOUR_VALUES[COLOUR_INDEX][2];
        }
        index++;
      }
    }
  }
  FastLED.show();
}

// Returns true if arr1 has equal values to arr2, else false
bool Snake::arrayEquals(const int arr1[], const int ARR_1_SIZE, const int arr2[], const int ARR_2_SIZE)
{
  if (ARR_1_SIZE != ARR_2_SIZE)
  {
    return false;
  }
  for (int i = 0; i < ARR_1_SIZE; i++)
  {
    if (arr1[i] != arr2[i])
    {
      return false;
    }
  }
  return true;
}

// Returns true if the matrix contains the supplied arr, else false
bool Snake::matrixContains(int matrix[][DIMENSIONS], const int MATRIX_SIZE, int arr[DIMENSIONS])
{
  for (int i = 0; i < MATRIX_SIZE; i++)
  {
    if (arrayEquals(matrix[i], DIMENSIONS, arr, DIMENSIONS))
    {
      return true;
    }
  }
  return false;
}

int Snake::findIndexOfString(const String STRING, const String arr[], const int ARR_SIZE)
{
  for (int i = 0; i < ARR_SIZE; i++)
  {
    if (arr[i].equals(STRING))
    {
      return i;
    }
  }
  return -1;
}
