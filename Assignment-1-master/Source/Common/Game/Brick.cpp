#include "Brick.h"
#include "Game.h"
#include "../Constants/Constants.h"
#include "../Screen Manager/ScreenManager.h"
#include "../OpenGL/OpenGL.h"

Brick::Brick() : GameObject()
{
	reset();
}

Brick::~Brick()
{

}

void Brick::update(double aDelta)
{
	// Bricks move downwards towards the paddle
	// Once they reach the bottom of the screeen the game is over
	float screenHeight = ScreenManager::getInstance()->getScreenHeight();
	setY(getY() + (5.0f * aDelta));

	if(getY() + getHeight() >= screenHeight)
	{
		Game* game = (Game*)ScreenManager::getInstance()->getScreenForName(GAME_SCREEN_NAME);
		//Safety check, 
		if(game != NULL)
		{
			game->gameOver();
		}
	}
}

void Brick::paint()
{
	
}

void Brick::paint(OpenGLTexture* texture)
{
	// Paint the brick texture
	OpenGLRenderer::getInstance()->drawTexture(texture, getX(), getY(), getWidth(), getHeight());

	if(m_HitStatus)
	{
		Game* game = (Game*)ScreenManager::getInstance()->getScreenForName(GAME_SCREEN_NAME);

		//Safety check, 
		if(game != NULL)
		{
			// Make sure the texture exists
			if(game->getTexture("brickCrack") != NULL)
			{
				OpenGLRenderer::getInstance()->drawTexture(game->getTexture("brickCrack"), getX(), getY(), getWidth(), getHeight());
			}
		}
	}
}

void Brick::reset()
{

	//Set the brick's width and height
	setWidth(GAME_BRICK_WIDTH);
	setHeight(GAME_BRICK_HEIGHT);
  
	//Reset the x and y position
	setX(GAME_BRICK_X_DEFAULT);
	setY(GAME_BRICK_Y_DEFAULT);

	// Reset the hit status of the brick
	m_HitStatus = false;

	//Reset the brick to active
	setIsActive(true);
}

const char* Brick::getType()
{
	return GAME_BRICK_TYPE;
}

void Brick::setSize(float aWidth, float aHeight)
{
	setWidth(aWidth);
	setHeight(aHeight);
}

void Brick::setWidth(float aWidth)
{
	m_Width = aWidth;
}

void Brick::setHeight(float aHeight)
{
	m_Height = aHeight;
}

void Brick::getSize(float &aWidth, float &aHeight)
{
	aWidth = getWidth();
    aHeight = getHeight();
}

float Brick::getWidth()
{
	return m_Width;
}

float Brick::getHeight()
{
	return m_Height;
}

bool Brick::getHitStatus()
{
	return m_HitStatus;
}

void Brick::setHitStatus(bool hit)
{
	m_HitStatus = hit;
}