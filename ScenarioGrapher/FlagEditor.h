#ifndef FLAGEDITOR_H
#define FLAGEDITOR_H
#include "InputBox.h"
#include "Connector.h"
#include "ScrollableRegion.h"
#include "FileManager.h"
#include <map>
#include <string>
#include <SFML/Graphics/Text.hpp>
#include <list>

#include "Button.h"

class FlagEditor
{
public:
	FlagEditor(Connector& conn, std::map<std::string, int>& localFlags,
		std::map<std::string, std::map<std::string,int>>& sharedFlags,
		sf::Vector2f availableSize, const sf::Font& font,
		const sf::Texture& buttonTexture, FileManager& fmgr);

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
	std::map<std::string, std::map<std::string,int>> &sharedFlags;

	std::list<sf::Text> requiredTexts;
	std::list<sf::Text> triggeredTexts;
	std::list<sf::Text> localTexts;
	std::list<sf::Text> sharedHeaders;
	std::list<sf::Text> sharedTexts;

	ScrollableRegion scrollbox;
	FileManager& fm;

	sf::Text breakTexts[4];
	Button* buttons[4];
	const unsigned int charSize = 12;
	int clickedButton;
	std::string inStrings[2];

	sf::RectangleShape rect;

	void moveTextBlock(TextBlocks block, float moveVal);
	void addText(std::list<sf::Text>& list);
	void addSharedText(const std::string& str);

	void increment(const sf::Vector2f& mousePos, std::list<sf::Text>& list,
				std::map<std::string, int>& map, bool set = true);

	void decrement(const sf::Vector2f& mousePos, std::list<sf::Text>& list,
				std::map<std::string, int>& map, bool set = true);

	void remove(const sf::Vector2f& mousePos, std::list<sf::Text>& list,
				std::map<std::string, int>& map);
};
#endif//FLAGEDITOR_H
