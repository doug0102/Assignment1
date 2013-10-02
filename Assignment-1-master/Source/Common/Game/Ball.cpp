//
//  Ball.cpp
//  GAM-1532 OSX Game
//
//  Created by Bradley Flood on 2013-01-09.
//  Copyright (c) 2013 Algonquin College. All rights reserved.
//

#include "Ball.h"
#include "Game.h"
#include "Paddle.h"
#include "Brick.h"
#include "../Constants/Constants.h"
#include "../Screen Manager/ScreenManager.h"
#include "../OpenGL/OpenGL.h"


Ball::Ball() : GameObject()
{
	reset();
}

Ball::~Ball()
{

}

void Ball::update(double aDelta)
{
	// If the ball hasn't been released from the paddle
	if(!m_Released)
	{
		Game* game = (Game*)ScreenManager::getInstance()->getScreenForName(GAME_SCREEN_NAME);
		if(game != NULL)
		{
			Paddle* paddle = (Paddle*)game->getGameObjectByType(GAME_PADDLE_TYPE);
			if(paddle != NULL)
			{
				// The ball follows the position of the paddle
				setX(paddle->getX() + (paddle->getWidth() - getRadius()) / 2);
				setY(paddle->getY() - getRadius());

				// Rotation will show the direction the ball will travel once released
				m_Rotation += getDirectionX() * (getSpeed() / GAME_BALL_ROTATION_RATIO);
				if(m_Rotation >= 360)
				{
					// Reset rotation 
					m_Rotation = 0;
				}
			}
		}
		return;
	}

	//Calculate the ball's x and y position
	setX(getX() + (getDirectionX() * getSpeed() * aDelta));
	setY(getY() + (getDirectionY() * getSpeed() * aDelta));

	//Vertical bounds check
	if(getY() - getRadius() < 0.0f)
	{
		//There was a collision at the top, reverse the y-direction
		setY(getRadius());
		setDirectionY(getDirectionY() * -1.0f);
	}
	else if(getY() - getRadius() > ScreenManager::getInstance()->getScreenHeight())
	{
		//The ball has gone off the bottom of the screen
		setIsActive(false);

		// Get the Game object from ScreenManager, and call reset. This will reset the level, not the game
		Game* game = (Game*)ScreenManager::getInstance()->getScreenForName(GAME_SCREEN_NAME);

		//Safety check, 
		if(game != NULL)
		{
			game->reset();
		}
	}

	//Horizontal bounds check
	if(getX() - getRadius() < 0.0f)
	{
		//There was a collision on the left side, reverse the x-direction
		setX(getRadius());
		setDirectionX(getDirectionX() * -1.0f);
	}
	else if(getX() + getRadius() > ScreenManager::getInstance()->getScreenWidth())
	{
		//There was a collision on the right side, reverse the x-direction
		setX(ScreenManager::getInstance()->getScreenWidth() - getRadius());
		setDirectionX(getDirectionX() * -1.0f);
	}

	// Rotate the ball texture
	// Rotate in the opposite direction when the ball changes its X direction
	// Rotation speed depends on the speed of the ball
	m_Rotation += getDirectionX() * (getSpeed() / GAME_BALL_ROTATION_RATIO);
	if(m_Rotation >= 360)
	{
		// Reset rotation 
		m_Rotation = 0;
	}
}

void Ball::paint()
{
	
}


void Ball::paint(OpenGLTexture* texture)
{
	OpenGLRenderer::getInstance()->drawTexture(texture, getX() - getRadius(), getY() - getRadius(), m_Rotation);
}

void Ball::reset()
{
	//Get the screen width and height
	float screenWidth = ScreenManager::getInstance()->getScreenWidth();
	float screenHeight = ScreenManager::getInstance()->getScreenHeight();

	//Reset the radius
	setRadius(screenWidth * GAME_BALL_RADIUS_PERCENTAGE);

	//Reset the x and y position
	//setX((screenWidth - getRadius()) / 2.0f);
	//setY((screenHeight - getRadius()) / 1.25f);

	//Reset the speed
	setSpeed(GAME_BALL_DEFAULT_SPEED);

	//Reset the direction
	setDirectionX(1.0f);
	setDirectionY(-1.0f);

	m_Rotation = 0;

	//Set the ball back to being active
	setIsActive(true);

	m_Released = false;
}

const char* Ball::getType()
{
	return GAME_BALL_TYPE;
}

void Ball::checkCollision(GameObject* aGameObject)
{
	//Make sure the ball is active, the gameObject isn't NULL and that it is also active
	if(getIsActive() == true && aGameObject != NULL && aGameObject->getIsActive() == true && m_Released == true)
	{
		//Determine what type the GameObject is
		if(strcmp(aGameObject->getType(), GAME_PADDLE_TYPE) == 0)
		{
			// If it's a paddle
			handlePaddleCollision((Paddle*)aGameObject);
		}
		else if(strcmp(aGameObject->getType(), GAME_BRICK_TYPE) == 0)
		{
			// If it's a brick
			handleBrickCollision((Brick*)aGameObject);
		}
	}
}

void Ball::handlePaddleCollision(Paddle* aPaddle)
{
	//Calculate the ball's distance from the paddle
	float distanceX = fabsf(getX() - aPaddle->getX() - (aPaddle->getWidth() / 2.0f));
	float distanceY = fabsf(getY() - aPaddle->getY() - (aPaddle->getHeight() / 2.0f));

	//If the distance on the x-axis is greater than half-the-width of the paddle + the ball's radius, then 
	//there is no way they can be colliding and return out of this method, no more collision handling is needed.
	if(distanceX > ((aPaddle->getWidth() / 2.0f) + getRadius()))
	{
		return; 
	}

	//If the distance on the y-axis is greater than half-the-height of the paddle + the ball's radius, then 
	//there is no way they can be colliding and return out of this method, no more collision handling is needed.
	if(distanceY > ((aPaddle->getHeight() / 2.0f) + getRadius())) 
	{
		return; 
	}

	//If we got here (passed the previous 2 if checks), then there is a good chance that a collision has occured.

	//If the distance on the x-axis is less than half-the-width of the paddle, then we have a collision on top of 
	//the paddle, set the ball's Y value and y-direction accordingly.
	if(distanceX <= (aPaddle->getWidth() / 2.0f))
	{

		Game* game = (Game*)ScreenManager::getInstance()->getScreenForName(GAME_SCREEN_NAME);

		//Safety check, 
		if(game != NULL)
		{
			if(game->getIsLeftMouseHeld())
			{
				m_Released = false;
			}
		}

		setY(aPaddle->getY() - getRadius());
		setDirectionY(getDirectionY() * -1.0f);
		return;
	} 

	//If the distance on the y-axis is less than half-the-height of the paddle, then we have a collision on either 
	//side of the paddle, set the x-direction accordingly.
	if(distanceY <= (aPaddle->getHeight() / 2.0f))
	{
		setDirectionX(getDirectionX() * -1.0f);
		return;
	}
}

void Ball::handleBrickCollision(Brick* aBrick)
{

	//Calculate the ball's distance from the brick
	float distanceX = fabsf(getX() - aBrick->getX() - (aBrick->getWidth() / 2.0f));
	float distanceY = fabsf(getY() - aBrick->getY() - (aBrick->getHeight() / 2.0f));

	// No collision
	if(distanceX > ((aBrick->getWidth() / 2.0f) + getRadius()))
	{
		return; 
	}

	// No collision
	if(distanceY > ((aBrick->getHeight() / 2.0f) + getRadius())) 
	{
		return; 
	}

	// Collision on top or bottom
	if(distanceX <= (aBrick->getWidth() / 2.0f))
	{
		if(getDirectionY() > 0)
		{
			// Hit top, correct the ball's position slightly
			// Helps prevent a glitch where bricks would sometimes be destroyed on first hit
			setY(getY() - 5);
		}

		if(getDirectionY() < 0)
		{
			// Hit bottom
			setY(getY() + 5);
		}

		// Reverse the ball's Y direction
		setDirectionY(getDirectionY() * -1.0f);

		// Check if the brick has been hit previously
		if(aBrick->getHitStatus())
		{
			// If it has, set to inactive
			aBrick->setIsActive(false);
			return;
		}

		// If the brick hasn't been hit yet, set the hit status of the brick to true
		aBrick->setHitStatus(true);
		return;
	} 

	// Collision on a side
	if(distanceY <= (aBrick->getHeight() / 2.0f))
	{
		if(getDirectionX() > 0)
		{
			// Hit left
			setX(getX() - 5);
		}

		if(getDirectionX() < 0)
		{
			// Hit right
			setX(getX() + 5);
		}

		// Reverse the ball's X direction and set brick to innactive
		setDirectionX(getDirectionX() * -1.0f);
		
		// Check if the brick has been hit previously
		if(aBrick->getHitStatus())
		{
			// If it has, set to inactive
			aBrick->setIsActive(false);
			return;
		}

		// If the brick hasn't been hit yet, set the hit status of the brick to true
		aBrick->setHitStatus(true);
		return;
	}
}


void Ball::setRadius(float aRadius)
{
	m_Radius = aRadius;
}

float Ball::getRadius()
{
	return m_Radius;
}

void Ball::setSpeed(float aSpeed)
{
	m_Speed = aSpeed;
}

float Ball::getSpeed()
{
	return m_Speed;
}

void Ball::setDirection(float aDirectionX, float aDirectionY)
{
	setDirectionX(aDirectionX);
	setDirectionY(aDirectionY);
}

void Ball::setDirectionX(float aDirectionX)
{
	m_DirectionX = aDirectionX;
}

void Ball::setDirectionY(float aDirectionY)
{
	m_DirectionY = aDirectionY;
}

void Ball::getDirection(float &aDirectionX, float &aDirectionY)
{
	aDirectionX = getDirectionX();
	aDirectionY = getDirectionY();
}

float Ball::getDirectionX()
{
	return m_DirectionX;
}

float Ball::getDirectionY()
{
	return m_DirectionY;
}

bool Ball::getReleased()
{
	return m_Released;
}

void Ball::setReleased(bool released)
{
	m_Released = released;
}
