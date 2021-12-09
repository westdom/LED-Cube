#include "Snake.h"
#include "Pattern.h"

// Constants
const int SNAKE_LENGTH = 3;
const int SIZE_OF_LEGAL_MOVES = 6;
String LEGAL_MOVES[SIZE_OF_LEGAL_MOVES] = {"LEFT", "RIGHT", "DOWN", "UP", "BACKWARDS", "FORWARDS"};

// Variables
int head[Pattern::DIMENSIONS] = {2, 0, 0};
int tail[SNAKE_LENGTH][Pattern::DIMENSIONS] = {
    {2, 0, 0},
    {1, 0, 0},
    {0, 0, 0}};
int pellet[Pattern::DIMENSIONS] = {2, 2, 2};
int positions[SNAKE_LENGTH + 1][Pattern::DIMENSIONS] = {
    {2, 0, 0},
    {1, 0, 0},
    {0, 0, 0},
    {2, 2, 2}};
uint32_t positionsColours[4];

Snake::Snake(const String SNAKE_NAME, const uint32_t SNAKE_COLOUR, const uint32_t PELLET_COLOUR, const bool PELLET_ON, const bool TRAVEL_THROUGH_WALLS_ON)
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
  this->snakeColour = CRGB::Black;
  this->pelletColour = CRGB::Magenta;
  this->pelletOn = PELLET_ON;
  this->travelThroughWallsOn = TRAVEL_THROUGH_WALLS_ON;
  this->rainbowSnake = true;
  setup();
}

void Snake::update()
{
  const unsigned long currentTime = millis();
  if (currentTime - previousUpdateStartTime >= DELAY)
  {
    previousUpdateStartTime = currentTime;
    const static int BACKGROUND_COLOUR = CRGB::Black;
    const static int NUMBER_OF_LEDS_TO_DISPLAY = pelletOn ? SNAKE_LENGTH + 1 : SNAKE_LENGTH;
    sendPositionsColours(leds, positions, NUMBER_OF_LEDS_TO_DISPLAY, positionsColours, BACKGROUND_COLOUR);
    moveSnakeHeadAndPellet();
    moveSnakeTail();
    setPositionsMatrix();
    if (rainbowSnake)
    {
      snakeColour = getNextRainbowColour();
      setPositionsColoursMatrix();
    }
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
  FastLED.setBrightness(MAX_BRIGHTNESS);
  randomSeed(analogRead(0));
  setPositionsColoursMatrix();
}

void Snake::setPositionsColoursMatrix()
{
  for (int i = 0; i < SNAKE_LENGTH; i++)
  {
    positionsColours[i] = snakeColour;
  }
  positionsColours[SNAKE_LENGTH] = pelletColour;
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