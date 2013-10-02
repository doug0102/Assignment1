#include "GameConstants.h"

//Game Constants
const char*  GAME_SCREEN_NAME = "Game";
const int    GAME_BRICKS_PER_ROW = 8;
const int    GAME_STARTING_LIVES = 3;
const float  GAME_SPEED_MOD = 1.25f;
const int	 GAME_MAX_ROUNDS = 5;

//Ball Constants
const float GAME_BALL_RADIUS_PERCENTAGE = 0.02f;
const float GAME_BALL_DEFAULT_SPEED = 350.0f;
const float GAME_BALL_ROTATION_RATIO = 100.0f;
const char* GAME_BALL_TYPE = "Ball";

//Paddle Constants
const float GAME_PADDLE_WIDTH_PERCENTAGE = 0.2f;
const float GAME_PADDLE_HEIGHT_PERCENTAGE = 0.025f;
const float GAME_PADDLE_Y_PERCENTAGE = 0.9f;
const char* GAME_PADDLE_TYPE = "Paddle";

// Brick Constants
const char* GAME_BRICK_TYPE = "Brick";
const float GAME_BRICK_HEIGHT = 30.0f;
const float GAME_BRICK_WIDTH = 100.0f;
const float GAME_BRICK_SPACE = 1.2f;
const float GAME_BRICK_X_DEFAULT = 35.0f;
const float GAME_BRICK_Y_DEFAULT = 35.0f;