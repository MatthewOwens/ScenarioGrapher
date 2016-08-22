#include "StateManager.h"
#include "GraphState.h"
#include <iostream>

StateManager::StateManager(InputManager* inputs, ImageManager* images, sf::RenderWindow* win)
{
	// Setting up our pointers
	inputManager = inputs;
	imageManager = images;
	window = win;
}

StateManager::StateManager()
{
	inputManager = NULL;
	imageManager = NULL;
	window = NULL;
}

StateManager::~StateManager()
{
	// Clearing our stack
	while(!stateStack.empty())
		popState();
}

void StateManager::pushState(StateEnum stateType, sf::Packet* data)
{
	// Pausing the current state, if needed
	if(!stateStack.empty())
		stateStack.top()->onPause();

	// Creating a new state on top of the stack
	switch(stateType)
	{
		case GRAPHER:
			stateStack.emplace(new GraphState());
			break;
		case CONNEDIT:
			break;
		case FLAGEDIT:
			break;
		default:
			std::cout << "Cannot create state!" << std::endl;
			return;
	}

	stateStack.top()->onEnter(data, imageManager);
	currentState = stateType;
}


// Removing the current state from the top of the stack
void StateManager::popState()
{
	stateStack.top()->onExit(imageManager);
	delete stateStack.top();
	stateStack.pop();
}

// Returns false if we have no more states to update
bool StateManager::update()
{
	// Processing events
	sf::Event event;
	while (window->pollEvent(event))
	{
		switch (event.type)
		{
			case sf::Event::Closed:
				window->close();
				break;

			default:
				break;
		}
	}

	inputManager->update(*window);

	// Just being safe
	if(!stateStack.empty())
	{
		stateStack.top()->update(inputManager, this);
	} 

	// Rechecking since we may have exited from within the state
	return stateStack.empty();
}

void StateManager::render()
{
	window->clear();

	if (!stateStack.empty() && window != NULL)
	{
		if (stateStack.size() > 1 && stateStack.top()->renderingPrevious())
		{
			BaseState* s = stateStack.top();
			stateStack.pop();

			stateStack.top()->render(window);
			stateStack.push(s);
		}
		stateStack.top()->render(window);
	}

	window->display();
}
