/*
 * The base state is an abstract class, intended to be extended with other classes
 * Contains a SFML packet to pass data between states, since there's not really much
 * point in reinventing the wheel.
 *
 * For more information on how states are manipulated, see StateManager.h
*/

#ifndef BASESTATE_H
#define BASESTATE_H
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Network/Packet.hpp>
#include "InputManager.h"
#include "ImageManager.h"

class StateManager;

class BaseState
{
public:
	virtual ~BaseState();// = 0;
	virtual void update(InputManager* inputManager, StateManager* stateManager) = 0;
	virtual void render(sf::RenderWindow* window) = 0;
	virtual void onEnter(sf::Packet* data, ImageManager* imageManager) = 0;
	virtual void onPause() = 0;
	virtual sf::Packet onExit(ImageManager* imageManager) = 0;
	inline bool renderingPrevious() { return renderPrevious; }

	// Packet used to transfer data between states
protected:
	BaseState(){};
	sf::Packet bundle;
	bool renderPrevious = false;
};
#endif //BASESTATE_H
