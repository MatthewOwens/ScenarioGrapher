#ifndef BASEVIEW_H
#define BASEVIEW_H
#include "Node.h"
#include <list>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <string>

/*! BaseView Interface class, to be inherited by both ModuleView and BaseView
 *
*/

class BaseView : public Node
{
	public:
		void render(sf::RenderWindow& window, bool showNames = true) {};
		void setID(const std::string& moduleID, int nodeNumber) {};
		bool removeRequired(const sf::Vector2f& mousePos);
		void move(const sf::Vector2f& vector);
		void setPosition(const sf::Vector2f& newPos);
		void update();
		void updateLines(const std::list<BaseView*>& nodeViews);
		void populateLines(const std::list<BaseView*>& nodeViews);
		sf::FloatRect getGlobalBounds();
		const sf::Vector2f& getPosition();
		void addConnector(Connector& connector, const sf::Vector2f& lineTarget);
		//void removeConnector(BaseView* target);
		void removeConnector(BaseView* target);

		inline const sf::Vector2f& getInletPos() { return circles[0].getPosition(); }
		inline const sf::Vector2f& getOutletPos() { return circles[1].getPosition(); }

		inline std::vector<sf::Text>& getPrioTexts() { return connectorPriorities; }
		inline std::vector<sf::Text>& getChoiceTexts() { return connectorTexts; }

	protected:
		sf::Font* font = NULL;
		sf::RectangleShape baseRect;
		sf::Text idText;

		sf::Vector2f position;
		sf::Vector2f size;
		sf::VertexArray lines;
		std::vector<sf::Text> connectorTexts;
		std::vector<sf::Text> connectorPriorities;

		sf::CircleShape circles[2];	//0 - inlet, 1 - outlet
		const float spacing = 5.f;
		const float textSpacingY = -35.f;
		const float circleSize = 5.f;
};
#endif//BASEVIEW_H
