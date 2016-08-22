#include "InputManager.h"
#include <iostream>
#include <functional>


InputManager::InputManager()
{
    keyBinds["up"] = sf::Keyboard::W;
    keyBinds["down"] = sf::Keyboard::S;
    keyBinds["left"] = sf::Keyboard::A;
    keyBinds["right"] = sf::Keyboard::D;
    keyBinds["confirm"] = sf::Keyboard::Return;
    keyBinds["cancel"] = sf::Keyboard::Escape;
	keyBinds["nextTurn"] = sf::Keyboard::P;
}

InputManager::~InputManager()
{
    //dtor
}

void InputManager::update(sf::RenderWindow& window)
{
	// Only update if we have focus
	if(!window.isOpen() || !window.hasFocus())
		return;
	
	// Updating the keyboard state
   	for(int i = 0; i < sf::Keyboard::KeyCount; i++)
   	{
       	previousPressedKeys[i] = pressedKeys[i];
       	pressedKeys[i] = sf::Keyboard::isKeyPressed( (sf::Keyboard::Key)i );
   	}

	// Updating the state of the mouse buttons
	for(int i = 0; i < sf::Mouse::ButtonCount; i++)
	{
		previousPressedMouseButtons[i] = pressedMouseButtons[i];
		pressedMouseButtons[i] = sf::Mouse::isButtonPressed( (sf::Mouse::Button)i );
	}

	// Updating the state of the mouse position
	previousMousePosition = mousePosition;
	//mousePosition = sf::Mouse::getPosition(window);
	sf::Vector2i pixelPos = sf::Mouse::getPosition(window);
	mousePosition = window.mapPixelToCoords(pixelPos);
}

bool InputManager::keyHeld(std::string keyName)
{
	return pressedKeys[keyBinds[keyName]];
}

bool InputManager::buttonHeld(sf::Mouse::Button button)
{
	return pressedMouseButtons[button];
}

bool InputManager::pressedOnce(std::string keyName)
{
    if(pressedKeys[keyBinds[keyName]] && !previousPressedKeys[keyBinds[keyName]])
        return true;
    else return false;
}

bool InputManager::pressedOnce(sf::Mouse::Button button)
{
	if(pressedMouseButtons[button] && !previousPressedMouseButtons[button])
		return true;
	else return false;
}

bool InputManager::validKeyDown()
{
    std::map<std::string, sf::Keyboard::Key>::iterator mapItr;

    for(mapItr = keyBinds.begin(); mapItr != keyBinds.end(); ++mapItr)
    {
        if(pressedOnce(mapItr->first) &&
           (mapItr->first != "confirm" || mapItr->first != "cancel"))
        {
            return true;
        }
    }
    return false;
}

// Returns true if the binding was successfully added, false if the key is already bound.
bool InputManager::addBinding(const std::string& bind, sf::Keyboard::Key key)
{
	//auto bindSearch = keyBinds.find(key);
	auto keySearch = std::find_if(keyBinds.begin(), keyBinds.end(),
								   std::bind2nd(MapFind(), key));

	if (keySearch == keyBinds.end())
	{
		keyBinds[bind] = key;
		return true;
	}
	else
	{
		std::cerr << "Cannot bind key " << key << " to " << bind << ", key is already bound!" << std::endl;
		return false;
	}
}

// Returns true if the binding was removed successfully, false if there was no such binding
bool InputManager::removeBinding(const std::string& bind)
{
	auto it = keyBinds.find(bind);
	bool foundKeys = false;

	while (it != keyBinds.end())
	{
		keyBinds.erase(it);
		it = keyBinds.find(bind);	// Searching again
	}
	return foundKeys;
}

// Returns true if the binding was removed successfully, false if there was no such binding
bool InputManager::removeBinding(sf::Keyboard::Key key)
{
	bool foundValues = false;
	auto keySearch = std::find_if(keyBinds.begin(), keyBinds.end(),
								   std::bind2nd(MapFind(), key));

	if (keySearch != keyBinds.end())
	{
		keyBinds.erase(keySearch);
		return true;
	}
	else return false;
}