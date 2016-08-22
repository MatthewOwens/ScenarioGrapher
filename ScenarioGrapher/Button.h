/// Simple class that extends SFML's sprite class with a few methods to check
/// when the sprite has been clicked

#ifndef BUTTON_H
#define BUTTON_H
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/String.hpp>
#include "InputManager.h"

class Button 
{
public:
	Button();
	Button(const sf::Vector2f size, const sf::Color color = sf::Color(255, 255, 255), const float outlineWidth = 0.f);
	Button(const sf::Texture &texture);
	Button(const sf::Texture &texture, const sf::IntRect &rectangle);
	~Button();

	inline bool isPressed() { return pressed; }
	inline bool isHeld() { return held; }
	inline bool isMouseOver() { return mouseOver; }

	void update(InputManager* inputManager);

	void setTexture(const sf::Texture &texture, bool resetRect = false);
	void setIconTexture(const sf::Texture& texture, bool resetRect = false);
	void setPosition(const sf::Vector2f &position);
	void setScale(const sf::Vector2f &factors);
	void setText(const sf::String &string, const sf::Font &font, unsigned int characterSize=20);
	void setColor(const sf::Color& color);
	void setHighlight(const sf::Color& color);
	void clearHighlight();

	const sf::Vector2f& getPosition();
	const sf::Vector2f& getScale();

	void rotate(float angle);
	void move(const sf::Vector2f& offset);
	void move(const float& offsetX, const float& offsetY);
	void scale(const sf::Vector2f& factor);

	void draw(sf::RenderWindow* window);
private:
	sf::Sprite* sprite = NULL;
	sf::RectangleShape* rectShape = NULL;
	//sf::Sprite* iconSprite = NULL;
	sf::Text* text = NULL;

	bool pressed = false;
	bool held = false;
	bool mouseOver = false;

	void updatePositions();
};
#endif//BUTTON_H
