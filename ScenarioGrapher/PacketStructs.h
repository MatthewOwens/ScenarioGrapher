#ifndef PACKETSTRUCTS_H
#define PACKETSTRUCTS_H
#include <SFML/Network/Packet.hpp>
#include <SFML/Graphics/Color.hpp>
#include "Button.h"
#include <stdint.h>
#include <map>
struct GraphStateData
{
	std::map<std::string, Button*>* buttons;
	sf::RenderWindow* window;
	sf::Font* font;
};

struct FlagStateData
{
	std::map<std::string, bool>* lflags;
	std::map<std::string, bool>* gflags;
};
#endif//PACKETSTRUCTS_H