#include "Game.h"
#include "GameObject.h"
#include "Paddle.h"
#include "Ball.h"
#include "Brick.h" 
#include "../OpenGL/OpenGL.h"
#include "../Constants/Constants.h"
#include "..//Utils/Utils.h"
#include "../Screen Manager/ScreenManager.h"

Game::Game()
{
	// Create all the textures used in the game

	m_Textures[0] = new OpenGLTexture("background");	// Background image

	// Number textures
	m_Textures[1] = new OpenGLTexture("1");				// #1
	m_Textures[2] = new OpenGLTexture("2");				// #2
	m_Textures[3] = new OpenGLTexture("3");				// #3
	m_Textures[4] = new OpenGLTexture("4");				// #4
	m_Textures[5] = new OpenGLTexture("5");				// #5
	m_Textures[6] = new OpenGLTexture("6");				// #6
	m_Textures[7] = new OpenGLTexture("7");				// #7
	m_Textures[8] = new OpenGLTexture("8");				// #8

	m_Textures[9] = new OpenGLTexture("pause");			// Pause texture
	m_Textures[10] = new OpenGLTexture("hp");			// Health texture
	m_Textures[11] = new OpenGLTexture("rnd");			// Round or "level" texture

	// Brick textures
	m_Textures[12] = new OpenGLTexture("brickBlue");	// Blue - Level 1
	m_Textures[13] = new OpenGLTexture("brickGreen");	// Green - Level 2
	m_Textures[14] = new OpenGLTexture("brickOrange");	// Orange - Level 3
	m_Textures[15] = new OpenGLTexture("BrickPurple");	// Purple - Level 4
	m_Textures[16] = new OpenGLTexture("brickRed");		// Red - Level 5
	m_Textures[17] = new OpenGLTexture("brickYellow");	// Yellow - Level 6

	m_Textures[18] = new OpenGLTexture("ball");			// The ball texture
	m_Textures[19] = new OpenGLTexture("gameOver");		// The game over screen texture

	m_Textures[20] = new OpenGLTexture("brickCrack");	// The brick cracking texture

	m_Textures[21] = new OpenGLTexture("title");		// The title screen

	// Call the newGame function
	// This will be called whenever a new game is to be played
	newGame();
	m_ShowTitle = true;
}

Game::~Game()
{
	// Delete all textures
	for(int i = 0; i < 22; i++)
	{
		delete m_Textures[i];
		m_Textures[i] = NULL;
	}

	//Delete all the GameObjects in the vector
	for(int i = 0; i < m_GameObjects.size(); i++)
	{
		delete m_GameObjects.at(i);
		m_GameObjects.at(i) = NULL;
	}

	//Clear the pointers from the vector
	m_GameObjects.clear();
}

void Game::update(double aDelta)
{
	//If the game is over, paused, or the title screen is showing
	if(m_Pause || m_GameOver || m_ShowTitle)
	{
		// Stops updating the game.
		return;
	}

	// Use an integer to keep track of the number of active Brick objects
	int bricksLeft = 0;

	//Cycle through all the game objects
	for(int i = 0; i < m_GameObjects.size(); i++)
	{
		//Make sure the GameObject is active
		if(m_GameObjects.at(i)->getIsActive() == true)
		{
			//Update the GameObject
			m_GameObjects.at(i)->update(aDelta);

			Ball* ball = (Ball*)getGameObjectByType(GAME_BALL_TYPE);
			if(ball->getIsActive())
			{
				ball->checkCollision(m_GameObjects.at(i));
			}

			// Increment the brick counter
			if(m_GameObjects.at(i)->getType() == GAME_BRICK_TYPE)	
			{
				bricksLeft++;
			}
		}
	}
	

	// Once no bricks are left (all inactive)
	if(bricksLeft == 0)
	{
		// Call the levelUp function
		levelUp();
	}
}

void Game::paint()
{
	// Show the title screen
	if(m_ShowTitle)
	{
		OpenGLRenderer::getInstance()->drawTexture(m_Textures[21], 15.0f, 0.0f);
		return;
	}

	// If the game is over
	if(m_GameOver)
	{
		// Draw the game over texture
		OpenGLRenderer::getInstance()->drawTexture(m_Textures[19], ScreenManager::getInstance()->getScreenWidth() / 2 - m_Textures[19]->getSourceWidth() / 2, ScreenManager::getInstance()->getScreenHeight() / 1.5);
		return;
	}

	// Draw the texture for the game background
	Paddle* paddle = (Paddle*)getGameObjectByType(GAME_PADDLE_TYPE);
	if(paddle != NULL)
	{
		// Move the textures as the paddle moves
		// Draw the background texture
		OpenGLRenderer::getInstance()->drawTexture(m_Textures[0], (paddle->getX() / 8) - (m_Textures[0]->getTextureWidth() / 4), 0.0f);

		// Only draw when player has lives
		if(m_Lives > 0)
		{
			// draws the HP texture
			OpenGLRenderer::getInstance()->drawTexture(m_Textures[10], paddle->getX() + paddle->getWidth() / 8, paddle->getY() + paddle->getHeight() * 1.55);
			OpenGLRenderer::getInstance()->drawTexture(m_Textures[m_Lives], paddle->getX() + paddle->getWidth() / 3.5, paddle->getY() + paddle->getHeight() * 1.25);
		}

		// Only draw when within the maximum number of rounds
		if(m_Round <= GAME_MAX_ROUNDS)
		{
			// draws the round texture
			OpenGLRenderer::getInstance()->drawTexture(m_Textures[11], paddle->getX() + paddle->getWidth() / 1.5, paddle->getY() + paddle->getHeight() * 1.55);
			OpenGLRenderer::getInstance()->drawTexture(m_Textures[m_Round + 1], paddle->getX() + paddle->getWidth() / 1.25, paddle->getY() + paddle->getHeight() * 1.25);
		}
	}

	//Cycle through and draw all the game objects
	for(int i = 0; i < m_GameObjects.size(); i++)
	{
		if(m_GameObjects.at(i)->getIsActive() == true)
		{
			// Painting bricks
			if(m_GameObjects.at(i)->getType() == GAME_BRICK_TYPE)
			{
				// Paints a different texture for each round
				m_GameObjects.at(i)->paint(m_Textures[12 + m_Round]);
			}

			// Painting balls
			else if(m_GameObjects.at(i)->getType() == GAME_BALL_TYPE)
			{
				m_GameObjects.at(i)->paint(m_Textures[18]);
			}
			else
			{
				m_GameObjects.at(i)->paint();
			}
		}
	}

	//Draw the outer white walls
	OpenGLRenderer::getInstance()->setForegroundColor(OpenGLColorWhite());
	OpenGLRenderer::getInstance()->setLineWidth(4.0f);
	OpenGLRenderer::getInstance()->drawLine(1.0f, 0.0f, 1.0f, getHeight());
	OpenGLRenderer::getInstance()->drawLine(0.0f, 1.0f, getWidth(), 1.0f);
	OpenGLRenderer::getInstance()->drawLine(getWidth() - 1, 0.0f, getWidth() - 1, getHeight());
	OpenGLRenderer::getInstance()->setLineWidth(1.0f);

	// If the game is paused
	if(m_Pause)
	{
		// Draw the pause texture
		OpenGLRenderer::getInstance()->drawTexture(m_Textures[9], ScreenManager::getInstance()->getScreenWidth() / 2 - m_Textures[9]->getSourceWidth() / 2, ScreenManager::getInstance()->getScreenHeight() / 1.5);
	}
}

void Game::reset()
{
	// Called when the ball is off screen (missed by paddle)
	// Reset the ball object

	// Subtract one life
	setLives(getLives() - 1);
	Ball* ball = (Ball*)getGameObjectByType(GAME_BALL_TYPE);
	if(ball != NULL)
	{
		float prevSpeed = ball->getSpeed();
		ball->reset();
		ball->setSpeed(prevSpeed);
	}

	// Once the player runs out of lives call the gameOver function
	if(getLives() <= 0)
	{
		gameOver();
	}
}

void Game::gameOver()
{
	m_GameOver = true;
}

const char* Game::getName()
{
	return GAME_SCREEN_NAME;
}

void Game::screenWillAppear()
{
	OpenGLRenderer::getInstance()->setBackgroundColor(OpenGLColorCornflowerBlue());
}

void Game::addGameObject(GameObject* aGameObject)
{
	if(aGameObject != NULL)
	{

		m_GameObjects.push_back(aGameObject);
	}
}

GameObject* Game::getGameObjectByType(const char* aType)
{
	//Cycle through a find the game object (if it exists)
	for(unsigned int i = 0; i < m_GameObjects.size(); i++)
	{
		if(strcmp(m_GameObjects.at(i)->getType(), aType) == 0)
		{
			return m_GameObjects.at(i);
		}
	}
	return NULL;
}

GameObject* Game::getGameObjectByMatch(GameObject* aGameObject)
{
	// Go through all GameObjects
	for(int i = 0; i < m_GameObjects.size(); i++)
	{
		// If they are of the same type, both active, and have the same X and Y location
		 if( m_GameObjects.at(i)->getType() == aGameObject->getType() &&
			 m_GameObjects.at(i)->getX() == aGameObject->getX() &&
			 m_GameObjects.at(i)->getY() == aGameObject->getY() &&
			 m_GameObjects.at(i)->getIsActive() == aGameObject->getIsActive())
		 {
			 return m_GameObjects.at(i);
		 }
	}
	return NULL;
}

void Game::mouseMovementEvent(float aDeltaX, float aDeltaY, float aPositionX, float aPositionY)
{
	// If the game isn't paused
	if(!m_Pause)
	{
		//Set the paddle to the x position of the mouse
		Paddle* paddle = (Paddle*)getGameObjectByType(GAME_PADDLE_TYPE);

		//Safety check, paddle could be NULL
		if(paddle != NULL)
		{
			paddle->setX(aPositionX - (paddle->getWidth() / 2.0f));
		}
	}
}

void Game::mouseLeftClickDownEvent(float positionX, float positionY)
{
	// If the ball has yet to be released, release it
	Ball* ball = (Ball*)getGameObjectByType(GAME_BALL_TYPE);
	if(ball != NULL)
	{
		if(!ball->getReleased())
		{
			ball->setReleased(true);
			return;
		}
	}

	m_LeftMouseHeld = true;
}

void Game::mouseLeftClickUpEvent(float positionX, float positionY)
{
	m_LeftMouseHeld = false;
}

void Game::mouseRightClickDownEvent(float positionX, float positionY)
{
	// If the ball has yet to be released
	Ball* ball = (Ball*)getGameObjectByType(GAME_BALL_TYPE);
	if(ball != NULL)
	{
		if(!ball->getReleased())
		{
			// Change the X direction of the ball
			ball->setDirectionX(ball->getDirectionX() * -1.0f);
		}
	}
}

void Game::newGame()
{	
	//Delete all the GameObject's in the vector
	for(int i = 0; i < m_GameObjects.size(); i++)
	{
		delete m_GameObjects.at(i);
		m_GameObjects.at(i) = NULL;
	}

	// Clear the pointers from the vector
	m_GameObjects.clear();

	// Adds a paddle and ball object to the vector
	addGameObject(new Paddle());
	addGameObject(new Ball());

	// Adds Brick objects to the vector
	// Creates 3 rows of bricks to start off
	for(int i = 0; i < GAME_BRICKS_PER_ROW * 3; i++)
	{
		addGameObject(new Brick());
	}

	//Cycle through and reset all the game objects
	for(int i = 0; i < m_GameObjects.size(); i++)
	{
		m_GameObjects.at(i)->reset();
	}

	// Reset the background image colour
	m_Textures[0]->setColor(OpenGLColorRGB(1.0f, 1.0f, 1.0f));

	// Unpause the game
	m_Pause = false;

	m_LeftMouseHeld = false;

	// Set the game over state to false
	m_GameOver = false;

	// Reset lives to 3
	setLives(GAME_STARTING_LIVES);

	// Reset the round
	m_Round = 0;

	// Place the bricks 
	placeBricks();
}

void Game::placeBricks()
{
	// This function evenly spaces Brick objects across the screen

	int bricks = 0;		// Keeps track of the number of bricks		
	int rows = 1;		// Keeps track of the number of rows

	// Go through all game objects
	for(int i = 0; i < m_GameObjects.size(); i++)
	{
		// If current object is a brick
		if(m_GameObjects.at(i)->getType() == GAME_BRICK_TYPE)
		{
			// If the number of bricks is a multiple of the size of a row
			// The size of a row is set to 8 by default
			if(bricks != 0 && bricks % GAME_BRICKS_PER_ROW == 0)
			{
				// Increment the row counter
				// Reset the brick counter
				rows++;
				bricks = 0;
			}

			// Space out the bricks
			m_GameObjects.at(i)->setX(GAME_BRICK_X_DEFAULT + (bricks * GAME_BRICK_WIDTH) * GAME_BRICK_SPACE);
			m_GameObjects.at(i)->setY((GAME_BRICK_Y_DEFAULT * rows) * GAME_BRICK_SPACE);
			bricks++;
		}
	}
}

void Game::levelUp()
{
	// This function is called when a player clears all bricks from the screen

	// Increase the number of lives by 1
	setLives(getLives() + 1);
	// Increase the rounds
	m_Round++;

	// If the current round is equal to or greater than the max rounds 
	if(m_Round > GAME_MAX_ROUNDS)
	{
		// Start a new game
		newGame();
	}

	// Sets the background image colour based on the current round
	switch(m_Round)
		{
		case 0:
			// Default
			m_Textures[0]->setColor(OpenGLColorRGB(1.0f, 1.0f, 1.0f));
			break;
		case 1:
			// Green
			m_Textures[0]->setColor(OpenGLColorRGB(0.5f, 1.0f, 0.5f));
			break;
		case 2:
			// Orange
			m_Textures[0]->setColor(OpenGLColorRGB(1.0f, 0.5f, 0.0f));
			break;
		case 3:
			// Purple
			m_Textures[0]->setColor(OpenGLColorRGB(0.5f, 0.5f, 0.0f));
			break;
		case 4:
			// Red
			m_Textures[0]->setColor(OpenGLColorRGB(1.0f, 0.5f, 0.5f));
			break;
		case 5:
			// Yellow
			m_Textures[0]->setColor(OpenGLColorRGB(1.0f, 1.0f, 0.0f));
			break;
		}

	// Add another row of bricks
	for(int i = 0; i < GAME_BRICKS_PER_ROW; i++)
	{
		addGameObject(new Brick());
	}


	Ball* ball = (Ball*)getGameObjectByType(GAME_BALL_TYPE);
	if(ball != NULL)
	{
		// Increases the ball's speed by 25% each time this function is called
		// Change GAME_SPEED_MOD in globals to change the speed increase per level
		float prevSpeed = ball->getSpeed();

		// Go through all game objects and reset them
		// Except for the paddle
		for(int i = 0; i < m_GameObjects.size(); i++)
		{
			if(m_GameObjects.at(i)->getType() != GAME_PADDLE_TYPE)
			{
				m_GameObjects.at(i)->reset();
			}
		}

		ball->setSpeed(prevSpeed * GAME_SPEED_MOD);
	}

	// Call the placeBrick function to space bricks
	// For a new round of play
	placeBricks();
}

void Game::keyUpEvent(int aKeyCode)
{
	// R = 82
	// Esc = 27
	// Space = 32

	// If the R key has been pressed (more specifically, released)
	if(m_GameOver || m_ShowTitle)
	{
		m_ShowTitle = false;
		newGame();
	}

	// If the space key is pressed
	if(aKeyCode == 32)
	{
		// Pauses/un-pauses the game
		if(!m_Pause)
		{
			m_Pause = true;
		}
		else
		{
			m_Pause = false;
		}
	}
}

unsigned short Game::getLives()
{
	return m_Lives;
}

void Game::setLives(unsigned short lives)
{
	m_Lives = lives;
}

unsigned short Game::getRound()
{
	return m_Round;
}

OpenGLTexture* Game::getTexture(std::string fileName)
{
	// Go through all textures
	for(int i = 0; i < 22; i++)
	{
		// Check for matching file name
		if(m_Textures[i]->getFilename() == fileName)
		{
			return m_Textures[i];
		}
	}
	// Return NULL if not found
	return NULL;
}

bool Game::getIsLeftMouseHeld()
{
	return m_LeftMouseHeld;
}