// Modified 18/90/2013
//	- Added Brick constants
// Modified 19/09/2013
//	- Added Game constants
//	- Added Ball constants

#ifndef GAME_CONSTANTS_H
#define GAME_CONSTANTS_H

//Game Constants
extern const char*  GAME_SCREEN_NAME;
extern const int    GAME_BRICKS_PER_ROW;
extern const int    GAME_STARTING_LIVES;
extern const float  GAME_SPEED_MOD;
extern const int	GAME_MAX_ROUNDS;	// Starts from 0

//Ball Constants
extern const float GAME_BALL_RADIUS_PERCENTAGE;
extern const float GAME_BALL_DEFAULT_SPEED;
extern const float GAME_BALL_ROTATION_RATIO;
extern const char* GAME_BALL_TYPE;

//Paddle Constants
extern const float GAME_PADDLE_WIDTH_PERCENTAGE;
extern const float GAME_PADDLE_HEIGHT_PERCENTAGE;
extern const float GAME_PADDLE_Y_PERCENTAGE;
extern const char* GAME_PADDLE_TYPE;

// Brick Constants
extern const char* GAME_BRICK_TYPE;
extern const float GAME_BRICK_HEIGHT;
extern const float GAME_BRICK_WIDTH;
extern const float GAME_BRICK_SPACE;
extern const float GAME_BRICK_X_DEFAULT;
extern const float GAME_BRICK_Y_DEFAULT;


#endif 