#include "ScrollableRegion.h"
#include <iostream>
#include "termcolor.hpp"

ScrollableRegion::ScrollableRegion(float left, float top, float width, float height, sf::Text* anchor)
{
	visBounds.setPosition(left, top);
	visBounds.setSize(sf::Vector2f(width, height));
	visBounds.setFillColor(sf::Color(22,22,22));

	this->anchor = anchor;
}

ScrollableRegion::ScrollableRegion()
{
	visBounds.setPosition(0,0);
	visBounds.setSize(sf::Vector2f(0,0));

	anchor = NULL;
}

void ScrollableRegion::addElement(sf::Text *x)
{
	elements.push_back(x);

	if(anchor == NULL)
	{
		std::cerr << termcolor::red << "error: no anchor found!\n" <<
		"\tanchor set to '" << x->getString().toAnsiString() << "'" <<
		termcolor::reset << std::endl;

		anchor = x;
	}

	//if(scrollCheck(anchor))
	//	visBounds.setFillColor(sf::Color::Red);

}
void ScrollableRegion::setAnchor(sf::Text* anchor)
{
	this->anchor = anchor;

	//if(scrollCheck(anchor))
	//	visBounds.setFillColor(sf::Color::Blue);
}

bool ScrollableRegion::scrollCheck(sf::Text *anchor)
{
	if(anchor == NULL)
		return false;

	// If we need to start scrolling
	const sf::FloatRect& anchorBounds = anchor->getGlobalBounds();
	const sf::FloatRect& rangeBounds  = visBounds.getGlobalBounds();

	if(anchorBounds.top + anchorBounds.height > rangeBounds.top + rangeBounds.height)
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

ScrollableRegion::~ScrollableRegion() {}
