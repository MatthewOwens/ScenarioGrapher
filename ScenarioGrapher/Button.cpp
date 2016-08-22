#include "Button.h"
#include <iostream>

Button::Button()
{
	//sprite = new sf::Sprite();
	rectShape = new sf::RectangleShape(sf::Vector2f(0.f, 0.f));
}
Button::Button(const sf::Texture &texture)
{
	sprite = new sf::Sprite(texture);
}

Button::Button(const sf::Texture &texture, const sf::IntRect &rectangle)
{
	sprite = new sf::Sprite(texture, rectangle);
}

Button::Button(const sf::Vector2f size, const sf::Color color, const float outlineWidth)
{
	rectShape = new sf::RectangleShape(size);
	rectShape->setFillColor(color);
	rectShape->setOutlineThickness(outlineWidth);
}

Button::~Button()
{
	// Cleaning everything up
	if(sprite != NULL)
	{
		delete sprite;
		sprite = NULL;
	}
	if(rectShape != NULL)
	{
		delete rectShape;
		rectShape = NULL;
	}
	if(text != NULL)
	{
		delete text;
		text = NULL;
	}
}

void Button::draw(sf::RenderWindow* window)
{
	if(sprite)
		window->draw(*sprite);
	if(rectShape)
		window->draw(*rectShape);
	if(text)
		window->draw(*text);
}

void Button::update(InputManager* inputManager)
{
	if(sprite)
		mouseOver = sprite->getGlobalBounds()
					.contains(inputManager->getMousePosition());
	else if (rectShape)
		mouseOver = rectShape->getGlobalBounds()
					.contains(inputManager->getMousePosition());
	else mouseOver = false;

	if(mouseOver)
	{
		held = inputManager->buttonHeld(sf::Mouse::Button::Left);
		pressed = inputManager->pressedOnce(sf::Mouse::Button::Left);
	}
	else
	{
		held = false;
		pressed = false;
	}

}

void Button::setColor(const sf::Color& color)
{
	if(sprite)
		sprite->setColor(color);

	if(rectShape)
		rectShape->setFillColor(color);
}

void Button::setTexture(const sf::Texture& texture, bool resetRect)
{
	if(sprite)
		sprite->setTexture(texture, resetRect);
}

void Button::setPosition(const sf::Vector2f& position)
{
	if(sprite)
		sprite->setPosition(position);
	if(rectShape)
		rectShape->setPosition(position);
	/*if(iconSprite)
		iconSprite->setPosition(position);*/
	updatePositions();
}

void Button::setScale(const sf::Vector2f& factors)
{
	if(sprite)
		sprite->setScale(factors);
	if(rectShape)
		rectShape->setScale(factors);
}

const sf::Vector2f& Button::getPosition()
{
	if(sprite)
		return sprite->getPosition();

	if(rectShape)
		return rectShape->getPosition();

	// Preventing things breaking
	else return sf::Vector2f(0.f, 0.f);
}

const sf::Vector2f& Button::getScale()
{
	if(sprite)
		return sprite->getScale();

	if(rectShape)
		return rectShape->getScale();
}

void Button::rotate(float angle)
{
	if(sprite)
		sprite->rotate(angle);
	if(rectShape)
		rectShape->rotate(angle);
}

void Button::move(const sf::Vector2f& offset)
{
	if(sprite)
		sprite->move(offset);
	if(rectShape)
		rectShape->move(offset);

	updatePositions();
}

void Button::move(const float& offsetX, const float& offsetY)
{
	if (sprite)
		sprite->move(offsetX, offsetY);
	if (rectShape)
		rectShape->move(offsetX, offsetY);

	updatePositions();
}

void Button::scale(const sf::Vector2f& factor)
{
	if(sprite)
		sprite->scale(factor);
	if(rectShape)
		rectShape->scale(factor);
}

void Button::setText(const sf::String &string, const sf::Font &font, unsigned int characterSize)
{
	if(!text)
		text = new sf::Text(string, font, characterSize);
	else	// No need to call new again if text already exists
	{
		text->setString(string);
		text->setFont(font);
		text->setCharacterSize(characterSize);
	}

	updatePositions();
}

void Button::setHighlight(const sf::Color& color)
{
	if(sprite)
		sprite->setColor(color);
	else rectShape->setOutlineColor(color);
}

void Button::clearHighlight()
{
	if(sprite)
		sprite->setColor(sf::Color::White);
	else rectShape->setOutlineColor(sf::Color(255, 255, 255, 0));
}

void Button::updatePositions()
{
	sf::Vector2f pos = getPosition();
	sf::FloatRect buttonBounds;
	float outlineOffset = 0.f;

	if(sprite)
		buttonBounds = sprite->getLocalBounds();
	else
	{
		buttonBounds = rectShape->getLocalBounds();
		outlineOffset = rectShape->getOutlineThickness();
	}

	// Centering the text
	if(text)
	{
		sf::FloatRect textBounds = text->getLocalBounds();
		text->setPosition(pos.x + (buttonBounds.width / 2) - (textBounds.width / 2) - outlineOffset,
						  pos.y + (buttonBounds.height / 2) - textBounds.height - outlineOffset);
	}
}
