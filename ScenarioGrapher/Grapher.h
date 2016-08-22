#ifndef GRAPHER_H
#define GRAPHER_H
#include <SFML/Graphics.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <list>
#include <map>
#include "NodeView.h"
#include "InputBox.h"
#include "FileManager.h"
#include "FlagEditor.h"
#include "ConnectionEditor.h"

#include "InputManager.h"
#include "ImageManager.h"
#include "Button.h"
#include "StateManager.h"
class Grapher
{
	public:
		enum InputState{ NONE, NAME, LOAD};
		Grapher();
		~Grapher();
		void run();
	
	private:
		void update();
		void render();
		int populateGraph(const std::vector<Node*>& nodes);

		// Helper functions to clean up the update() method
		void onLeftClick(sf::Vector2f& viewPos);
		void onLeftRelease();

		void onMiddleClick(sf::Vector2f& viewPos);
		void onMiddleRelease(sf::Vector2f& viewPos);

		void onRightClick(sf::Vector2f& viewPos);
		void onRightRelease();

		void onTextEntered(int unicode);
		void onMouseScroll(float delta);

		bool close = false;
		InputState inState = NONE;
		sf::View graphView;

		InputManager inputManager;
		ImageManager imageManager;
		StateManager stateManage;

		FileManager fileManager;
		InputBox ibox;				// The InputBox used to get input for this class
		sf::Text moduleName;		// Name of the module being edited, defaults to untitled
		sf::Font font;
		sf::RectangleShape graphBG;	// Box to the left of the screen, just there
									// so we have a place to put the UI
		std::map<std::string, sf::Color> colors;	// UI Colors
		std::map<std::string, Button*> buttons;		// UI Buttons
		std::list<NodeView*> nodeViews;		// Representations of the nodes in this module
		sf::RenderWindow window;
		sf::Event event;
		int nodeCount;
		float scaleFactor;
		float scale;
		bool movingView;
		bool showNodeNames;

		std::map<std::string, bool> lFlags;
		std::map<std::string, bool> gFlags;

		NodeView* selectedNode;
		NodeView* clickedNode;
		InputBox* selectedInputBox;

		NodeView* connectingNodes[2];	// Array of pointers for storing the nodes
										// are to be connected with a new connection

		FlagEditor* flagEdit;
		ConnectionEditor* connEdit;
		sf::Vector2f connSpawn;
		sf::Clock clock;
};
#endif//GRAPHER_H
