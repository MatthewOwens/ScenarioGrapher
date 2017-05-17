/// Class to define a scrollable region, currently only supports scrolling
/// through sf::Text items.
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
		ScrollableRegion(float left, float top, float width, float height, sf::Text* anchor);
		~ScrollableRegion();
		sf::RectangleShape& getVisBounds() { return visBounds; }
		void addElement(sf::Text *x);
		void setAnchor(sf::Text* anchor);

		void resizeBounds(sf::Text *x = NULL);
		void scroll(float sfDelta, float val = 3.0f);

	private:
		sf::RectangleShape visBounds;

		std::vector<sf::Text*> elements;
		sf::Text* anchor;				// Anchor who's position we'll be checking against

		bool scrollCheck(sf::Text *anchor);
};
#endif//SCROLLABLEREGION_H
