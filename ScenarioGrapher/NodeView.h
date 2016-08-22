/// Class to display nodes and connectors and provide an interface to edit
/// them.
#ifndef NODEVIEW_H
#define NODEVIEW_H
#include "Node.h"
#include "InputBox.h"
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/VertexArray.hpp>
#include <string>
#include <list>
class NodeView
{
	public:
		NodeView(const std::string& moduleID, int nodeCount,
				 const sf::Vector2f position, sf::Font& font);

		NodeView(const sf::Vector2f position, sf::Font& font,
				 Node* node);
		~NodeView();
		void render(sf::RenderWindow& window, bool showNames = true);
		void setID(const std::string& moduleID, int nodeNumber);
		bool removeRequired(const sf::Vector2f& mousePos);
		void move(const sf::Vector2f& vector);
		void setPosition(const sf::Vector2f& newPos);
		void updateLines(const std::list<NodeView*>& nodeViews);
		void populateLines(const std::list<NodeView*>& nodeViews);
		void updateNodeText();
		void update();
		InputBox* getSelectedInputBox(const sf::Vector2f& mousePos);
		InputBox* getOtherInputBox(InputBox* selected);
		std::string getUnsavedInput();
		sf::FloatRect getGlobalBounds();
		const std::string& getID();
		const sf::Vector2f& getPosition();
		inline Node* getNode() { return node; }
		inline const sf::Vector2f& getInletPos() { return circles[0].getPosition(); }
		inline const sf::Vector2f& getOutletPos() { return circles[1].getPosition(); }
		bool addConnector(Connector& connector, const sf::Vector2f& lineTarget);
		void removeConnector(NodeView* target);

		inline std::vector<sf::Text>& getPrioTexts() { return connectorPriorities; }
		inline std::vector<sf::Text>& getChoiceTexts() { return connectorTexts; }
	private:
		// Enum for accessing the text and rect arrays
		enum {ID, HEADER, BODY};

		Node* node = NULL;
		sf::Font* font = NULL;
		sf::RectangleShape baseRect;
		sf::CircleShape circles[2];	//0 - inlet, 1 - outlet
		sf::Text idText;
		InputBox headerInput;
		InputBox bodyInput;

		sf::Vector2f position;
		sf::Vector2f size;
		sf::VertexArray lines;
		std::vector<sf::Text> connectorTexts;
		std::vector<sf::Text> connectorPriorities;
		const float spacing = 5.f;
		const float textSpacingY = -35.f;
		const float circleSize = 5.f;
};
#endif//NODEVIEW_H
