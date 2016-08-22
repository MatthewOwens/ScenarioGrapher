#include "GraphState.h"
#include "PacketStructs.h"
#include <iostream>

GraphState::GraphState()
{
	buttons = NULL;
	window = NULL;

	nodeCount = 0;
	showingNodeNames = true;
	scale = 1.f;
	scaleFactor = 1.f;

	for (int i = 0; i < 2; ++i)
		connectingNodes[i] = NULL;

}


GraphState::~GraphState()
{
}

void GraphState::processEvents(const sf::Event& event)
{
	switch (event.type)
	{
		case sf::Event::TextEntered:
		{
			if (selectedInputBox != NULL)
			{
				// ASCII
				if (event.text.unicode < 128 && event.text.unicode > 31)
					selectedInputBox->addCharacter(static_cast<char>(event.text.unicode));
				else if (event.text.unicode == 8)	// Backspace
					selectedInputBox->removeCharacter();
				else if (event.text.unicode == 13)	// Return
				{
					NodeView* n = NULL;

					for (auto i : nodeViews)
					{
						if (selectedInputBox == i->getBodyBox() || selectedInputBox == i->getHeaderBox())
						{
							n = i;
							break;
						}
					}

					selectedInputBox->setSelected(false);
					n->updateNodeText();
				}

			}
			break;
		}

		case sf::Event::MouseButtonPressed:
		{
			switch (event.mouseButton.button)
			{
			case sf::Mouse::Button::Left:
				onLeftClick();
				break;
			case sf::Mouse::Button::Right:
				onRightClick();
				break;
			case sf::Mouse::Button::Middle:
				onMiddleClick();
				break;
			}
			break;
		}

		case sf::Event::MouseButtonReleased:
		{
			if (event.mouseButton.button == sf::Mouse::Button::Middle)
				onMiddleRelease();
			break;
		}
		case sf::Event::MouseWheelScrolled:
		{
			// Calculating the scale factor
			if (event.mouseWheelScroll.delta < 0)
				scaleFactor = 0.9f;
			else if (event.mouseWheelScroll.delta > 0)
				scaleFactor = 1.1f;
			else scaleFactor = 1.0f;

			graphView.zoom(scaleFactor);
			break;
		}
	}
}

void GraphState::update(InputManager* inputManager, StateManager* stateManager)
{
	viewMousePos = window->mapPixelToCoords((sf::Vector2i)inputManager->getMousePosition(), graphView);

	if (inputManager->pressedOnce("cancel"))
	{
		// Clearing the input box
		if (selectedInputBox != NULL)
		{
			NodeView* n = NULL;

			for (auto i : nodeViews)
			{
				if (selectedInputBox == i->getHeaderBox() || selectedInputBox == i->getBodyBox())
				{
					n = i;
					break;
				}
			}

			selectedInputBox->setString(n->getUnsavedInput);
			selectedInputBox->setSelected(false);
			selectedInputBox = NULL;
		}

		// Clearing the dragged nodeview if needed
		if (clickedNode != NULL)
			clickedNode = NULL;
	}

	// If we've selected a node
	if (selectedNode != NULL)
	{
		// TODO: Move to Connection edit state
		std::cout << "Selected a node!" << std::endl;
		selectedNode = NULL;
	}
}

void GraphState::onLeftClick()
{
	float clickTime = clickClock.restart().asMilliseconds();

	for (auto i : nodeViews)
	{
		bool selectedMe = false;

		// Checking to see if we've clicked a node / input box
		selectedInputBox = i->getSelectedInputBox(viewMousePos);

		if (selectedInputBox != NULL)
			selectedMe = true;

		if (i->getGlobalBounds().contains(viewMousePos) || selectedMe)
		{
			if (clickTime < 250)
				selectedNode = i;
			else clickedNode = i;
		}
		break;
	}
}

void GraphState::onRightClick()
{
	//for (auto i : nodeViews)
	for (auto i = nodeViews.begin(); i != nodeViews.end() ; )
	{
		if((*i)->removeRequired(viewMousePos))
		{
			// Ensuring clean connections
			for (auto j : nodeViews)
				if (j != *i) j->removeConnector(*i);

			// Clearing selections
			clickedNode = NULL;
			connectingNodes[0] = NULL;
			connectingNodes[1] = NULL;
			if (selectedInputBox != NULL)
			{
				selectedInputBox->setSelected(false);
				selectedInputBox = NULL;
			}

			delete *i;
			*i = NULL;
			i = nodeViews.erase(i);
		}
		else ++i;
	}

	for (auto i : nodeViews)
		i->populateLines(nodeViews);
}

void GraphState::onMiddleClick()
{
	connectingNodes[0] = NULL;

	for (auto i : nodeViews)
	{
		if (i->getGlobalBounds().contains(viewMousePos))
		{
			std::cout << "Node Connection started" << std::endl;
			connectingNodes[0] = i;
			break;
		}
	}

	if (connectingNodes[0] == NULL)
		std::cout << "No nodeview found!" << std::endl;
}

void GraphState::onMiddleRelease()
{
	// If we're making a connection
	if (connectingNodes[0] != NULL)
	{
		for (auto i : nodeViews)
		{
			if (i->getGlobalBounds().contains(viewMousePos)
				&& i != connectingNodes[0])
			{
				connectingNodes[1] = i;

				std::cout << "connecting " << connectingNodes[0]->getID();
				std::cout << " and " << connectingNodes[1]->getID() << std::endl;
				break;
			}
		}

		if (connectingNodes[1] == NULL)
			std::cout << "No node to connect to!" << std::endl;
		else
		{
			Connector connection(connectingNodes[0]->getNode(), connectingNodes[1]->getNode());

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::F))
				std::cout << "Flags are under construction!" << std::endl;
			else
			{
				if (connectingNodes[0]->addConnector(connection, connectingNodes[1]->getInletPos()))
					std::cout << "Connection completed successfully!" << std::endl;
				else std::cout << "Connection failed!" << std::endl;
			}
		}

		// Clearing the connections
		connectingNodes[0] = NULL;
		connectingNodes[1] = NULL;
	}
}

void GraphState::render(sf::RenderWindow* window)
{
	window->setView(graphView);

	for (auto i : nodeViews)
		i->render(*window, showingNodeNames);

	window->setView(window->getDefaultView());
}

void GraphState::onEnter(sf::Packet* data, ImageManager* imageManager)
{
	GraphStateData* gsd = (GraphStateData*)data->getData();
	buttons = gsd->buttons;
	window = gsd->window;
	font = gsd->font;

	graphView.reset(sf::FloatRect(0,0, window->getSize().x, window->getSize().y));
}

void GraphState::onPause()
{
}

int GraphState::populateGraph(const std::vector<Node*>& nodes)
{
	if (nodes.size() == 0)
	{
		std::cerr << "no nodes in file!" << std::endl;
		return 1;
	}

	sf::Vector2f spawnPos = window->mapPixelToCoords(sf::Vector2i(50,50), graphView);
	std::list<NodeView*> openSet;
	std::vector<NodeView*> closedSet;

	// Clearing any preexisting nodeViews
	for (auto i = nodeViews.begin(); i != nodeViews.end(); )
	{
		delete *i;
		*i = NULL;
		i = nodeViews.erase(i);
	}

	for (int i = 0; i < 2; ++i)
		connectingNodes[i] = NULL;

	for (auto i : nodes)
	{
		nodeViews.push_back(new NodeView(spawnPos, *font, i));
	}

	for (auto i : nodeViews)
	{
		i->populateLines(nodeViews);
		i->update();
	}

	// Positioning the nodeViews correctly
	openSet.push_back(nodeViews.front());
	sf::Vector2f gap(150, 300);

	while (!openSet.empty())
	{
		std::vector<Connector>& conns = openSet.front()->getNode()->getConnections();
		int connCount = 0;

		for (auto conn : conns)
		{
			for (auto nodeView : nodeViews)
			{
				/* If the nodeview associated with conn isn't in the open or closed sets
				 * reposition the nodeView and push it to the open set
				*/
				if (conn.getEnd() == nodeView->getNode() &&
					std::find(openSet.begin(), openSet.end(), nodeView) == openSet.end() &&
					std::find(closedSet.begin(), closedSet.end(), nodeView) == closedSet.end())
				{
					nodeView->setPosition(openSet.front()->getOutletPos());
					nodeView->move(sf::Vector2f(gap.x, gap.y * connCount));
					openSet.push_back(nodeView);
					break;
				}
			}

			connCount++;
		}
		openSet.pop_front();
	}

	for (auto i : nodeViews)
		i->updateLines(nodeViews);


	nodeCount = nodeViews.size();
	return 0;
}

void GraphState::onExit()
{
	connectingNodes[0] = NULL;
	connectingNodes[1] = NULL;
	selectedNode = NULL;
	selectedInputBox = NULL;

	for (auto i = nodeViews.begin(); i != nodeViews.end(); )
	{
		delete *i;
		*i = NULL;
		i = nodeViews.erase(i);
	}

	buttons = NULL;
	window = NULL;
	font = NULL;
}