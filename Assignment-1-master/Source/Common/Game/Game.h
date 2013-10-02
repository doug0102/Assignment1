//
//  Game.h
//  GAM-1532 OSX Game
//
//  Shea Dougherty-Gill 
//	September 2013
// 
// 

#ifndef GAME_H
#define GAME_H

#include <vector>
#include "../Screen Manager/Screen.h"

class GameObject;
class OpenGLTexture;

class Game : public Screen
{
public:
	Game();
	~Game();

	//Game lifecycle methods
	void update(double delta);
	void paint();
	void reset();

	//Game Over method, call this when to end the game
	void gameOver();

	//Screen name, must be implemented, it's a pure
	//virtual method in the Screen class
	const char* getName();

	//Screen event method, inherited from the screen class
	void screenWillAppear();

	//GameObject Methods
	void addGameObject(GameObject* gameObject);					// Add a GameObject to the vector
	GameObject* getGameObjectByType(const char* type);			// Retrieve a GameObject based on type
	GameObject* getGameObjectByMatch(GameObject* gameObject);	// Retrieve a GameObject based on matching variables

	// Getter and setter for m_Lives
	unsigned short getLives();
	void setLives(unsigned short lives);

	// Getter for m_Round
	unsigned short getRound();

	// Getter for the left mouse button being held down
	bool getIsLeftMouseHeld();

	// Returns a pointer to a texture within the game
	OpenGLTexture* getTexture(std::string fileName);

private:

	// Mouse Events
	void mouseMovementEvent(float deltaX, float deltaY, float positionX, float positionY);
	void mouseLeftClickDownEvent(float positionX, float positionY);
	void mouseLeftClickUpEvent(float positionX, float positionY);
	void mouseRightClickDownEvent(float positionX, float positionY);

	// Key Events
	void keyUpEvent(int keyCode);

	// Vector to hold the GameObjects
	std::vector<GameObject*> m_GameObjects;

	// Creates a new game
	void newGame();

	// Places Brick objects on the screen evenly spaced
	void placeBricks();

	// Sets objects and variables for level up
	void levelUp();

	// Boolean for pausing the game
	bool m_Pause;

	// Boolean for the game over menu
	bool m_GameOver;

	// Boolean used for the game title screen
	bool m_ShowTitle;

	// Keeps track of number of balls left
	unsigned short m_Lives;

	// Keeps track of the current round (level)
	unsigned short m_Round;

	// Textures
	OpenGLTexture* m_Textures[22];

	bool m_LeftMouseHeld;
};

#endif