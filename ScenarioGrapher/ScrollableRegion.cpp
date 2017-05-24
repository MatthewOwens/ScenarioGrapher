#include "ScrollableRegion.h"
#include <iostream>
#include "termcolor.hpp"

ScrollableRegion::ScrollableRegion(float left, float top, float width, float height,
	   								sf::Transformable* anchor, sf::Transformable* topAnchor)
{
	visBounds.setPosition(left, top);
	visBounds.setSize(sf::Vector2f(width, height));
	visBounds.setFillColor(sf::Color(22,22,22));

	this->anchor = anchor;
	this->topAnchor = topAnchor;
}

ScrollableRegion::ScrollableRegion()
{
	visBounds.setPosition(0,0);
	visBounds.setSize(sf::Vector2f(0,0));

	anchor = NULL;
}

void ScrollableRegion::addElement(sf::Transformable *x)
{
	elements.push_back(x);

	if(anchor == NULL)
		anchor = x;

	//if(scrollCheck(anchor))
	//	visBounds.setFillColor(sf::Color::Red);

}
void ScrollableRegion::setAnchor(sf::Transformable* anchor)
{
	this->anchor = anchor;

	//if(scrollCheck(anchor))
	//	visBounds.setFillColor(sf::Color::Blue);
}

bool ScrollableRegion::scrollCheck(sf::Transformable *anchor)
{
	sf::Text* text = dynamic_cast<sf::Text*>(anchor);
	sf::Text* topText = dynamic_cast<sf::Text*>(topAnchor);

	if(anchor == NULL || text == NULL ||
	topAnchor == NULL || topText == NULL)
		return false;

	// If we need to start scrolling
	const sf::FloatRect& anchorBounds = text->getGlobalBounds();
	const sf::FloatRect& topAnchorBounds = topText->getGlobalBounds();
	const sf::FloatRect& rangeBounds  = visBounds.getGlobalBounds();

	if((anchorBounds.top + anchorBounds.height > rangeBounds.top + rangeBounds.height)
			|| topAnchorBounds.top < rangeBounds.top )
		return true;
	else return false;
}

void ScrollableRegion::scroll(float scrollVal, float val)
{
	if(!scrollCheck(anchor))
		return;

	// Normalising the scrollVal
	scrollVal = (scrollVal > 0 ? 1.0f : -1.0f);

	val *= scrollVal;
	sf::Vector2f newPos;

	for(auto itr : elements)
	{
		newPos = itr->getPosition();
		newPos.y += val;
		itr->setPosition(newPos);
	}
}

void ScrollableRegion::removeElement(sf::Transformable* x)
{
	// Removing the element from our vector if possible
	for(auto i = elements.begin(); i != elements.end(); ++i)
	{
		if(x == *i)
		{
			elements.erase(i);
			break;
		}
	}

	// Resetting anchors to sensible values if need be
	if(x == topAnchor)
		x = elements.front();
	else if (x == anchor)
		x = elements.back();
}

ScrollableRegion::~ScrollableRegion() {}
