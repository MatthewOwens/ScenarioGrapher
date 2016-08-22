#include "BaseState.h"
#include <iostream>

//BaseState::BaseState(){}
BaseState::~BaseState(){}
void BaseState::update(InputManager* inputManager, StateManager* stateManager){}
void BaseState::render(sf::RenderWindow* window){}
void BaseState::onEnter(sf::Packet* data, ImageManager* imageManager){}
void BaseState::onPause(){}
void BaseState::processEvents(const sf::Event& event) {}
sf::Packet BaseState::onExit(ImageManager* imageManager){return bundle;}
