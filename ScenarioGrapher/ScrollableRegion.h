/// Class to define a scrollable region, currently only supports scrolling
/// through sf::Transformable items.
///
/// ScrollableRegion is merely a way to view the elements within. The region itself does
/// own anything in the 'elements' vector.

#ifndef SCROLLABLEREGION_H
#define SCROLLABLEREGION_H
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/Text.hpp>
#include <vector>

class ScrollableRegion
{
	public:
		ScrollableRegion();
		ScrollableRegion(float left, float top, float width, float height, sf::Transformable* anchor);
		~ScrollableRegion();
		sf::RectangleShape& getVisBounds() { return visBounds; }
		void addElement(sf::Transformable *x);
		void setAnchor(sf::Transformable* anchor);

		void resizeBounds(sf::Transformable *x = NULL);
		void scroll(float sfDelta, float val = 3.0f);

	private:
		sf::RectangleShape visBounds;

		std::vector<sf::Transformable*> elements;
		sf::Transformable* anchor;				// Anchor who's position we'll be checking against

		bool scrollCheck(sf::Transformable *anchor);
};
#endif//SCROLLABLEREGION_H
