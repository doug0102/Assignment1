//
//  Brick.h
//  GAM-1532 OSX Game
//  Shea Dougherty-Gill
//	18/09/2013
// 

#ifndef BRICK_H
#define BRICK_H

class Game;

#include "GameObject.h"

// Brick class, inherits from GameObject
class Brick : public GameObject
{
public:
	Brick();
	~Brick();

	//Implementing the pure virtual lifecycle methods from GameObject
	void update(double delta);
	void paint();

	// Overloaded paint function to paint different textures each round
	void paint(OpenGLTexture* texture);

	//Overriding the virtual reset method from GameObject
	void reset();

	//Implementing the pure virtual type method from GameObject
	const char* getType();

	//Setter size methods
	void setSize(float width, float height);
	void setWidth(float width);
	void setHeight(float height);
  
	//Getter size methods
	void getSize(float &width, float &height);
	float getWidth();
	float getHeight();

	// Getter and setter for the brick's hit status
	bool getHitStatus();
	void setHitStatus(bool hit);

protected:

	bool m_HitStatus;
	float m_Width;
	float m_Height;
};

#endif