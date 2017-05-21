#ifndef FLAGEDITOR_H
#define FLAGEDITOR_H
#include "InputBox.h"
#include "Connector.h"
#include "ScrollableRegion.h"
#include <map>
#include <string>
#include <SFML/Graphics/Text.hpp>
#include <list>

#include "Button.h"

class FlagEditor
{
public:
	FlagEditor(Connector& conn, std::map<std::string, int>& localFlags,
		std::map<std::string, int>& globalFlags, sf::Vector2f availableSize,
		const sf::Font& font, const sf::Texture& buttonTexture);

	~FlagEditor();
	void render(sf::RenderWindow& window);
	bool checkButtons(InputManager* inputManager);
	void incrementFlags(const sf::Vector2f& mousePos);
	void inputString(std::string str);
	void decrementFlags(const sf::Vector2f& mousePos);
	void removeFlags(const sf::Vector2f& mousePos);
	inline bool gettingText() { return (inStrings[0] == "" ? false : true); }
	void checkScroll(float delta);
private:
	enum TextBlocks {LOCAL = 0, GLOBAL, REQUIRED, TRIGGERED};

	Connector& conn;	// The connection who's flags we're going to be editing
	const sf::Font& fnt;
	std::map<std::string, int> &localFlags;
	std::map<std::string, int> &globalFlags;

	std::list<sf::Text> requiredTexts;
	std::list<sf::Text> triggeredTexts;
	std::list<sf::Text> localTexts;
	std::list<sf::Text> globalTexts;

	ScrollableRegion scrollbox;

	sf::Text breakTexts[4];
	Button* buttons[4];
	const unsigned int charSize = 12;
	int clickedButton;
	std::string inStrings[2];

	sf::RectangleShape rect;

	void moveTextBlock(TextBlocks block, float moveVal);
	void addText(std::list<sf::Text>& list);

	void increment(const sf::Vector2f& mousePos, std::list<sf::Text>& list,
				std::map<std::string, int>& map, bool set = true);

	void decrement(const sf::Vector2f& mousePos, std::list<sf::Text>& list,
				std::map<std::string, int>& map, bool set = true);

	void remove(const sf::Vector2f& mousePos, std::list<sf::Text>& list,
				std::map<std::string, int>& map);
};
#endif//FLAGEDITOR_H
