#ifndef GRAPHSTATE_H
#define GRAPHSTATE_H
#include "BaseState.h"
#include "NodeView.h"
#include "Button.h"
#include "Node.h"
class GraphState :
	public BaseState
{
public:
	GraphState();
	~GraphState();
	void processEvents(const sf::Event& event);
	void update(InputManager* inputManager, StateManager* stateManager);
	void render(sf::RenderWindow* window);
	void onEnter(sf::Packet* data, ImageManager* imageManager);
	void onPause();
	void onExit();
private:
	int populateGraph(const std::vector<Node*>& nodes);
	void onLeftClick();
	void onRightClick();
	void onMiddleClick();

	void onMiddleRelease();


	sf::View graphView;
	sf::Clock clickClock;
	std::list<NodeView*> nodeViews;
	NodeView* connectingNodes[2];
	NodeView* clickedNode;
	NodeView* selectedNode;
	InputBox* selectedInputBox;

	sf::View graphView;
	int nodeCount;
	float scale;
	float scaleFactor;
	bool showingNodeNames;

	sf::Vector2f viewMousePos;

	// Passed from Grapher
	std::map<std::string, Button*>* buttons;
	sf::RenderWindow* window;
	sf::Font* font;

};
#endif//GRAPHSTATE_H