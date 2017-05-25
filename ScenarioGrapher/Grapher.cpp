#include "Grapher.h"
#include <iostream>
#include <algorithm>
Grapher::Grapher() : fileManager("dialogue/")
{
	window.create(sf::VideoMode(1280, 720), "Scenario Grapher");

	// Loading the font
	font.loadFromFile("assets/fonts/Inconsolata.otf");

	nodeCount = 0;
	scale = 1.f;
	selectedNode = NULL;
	selectedInputBox = NULL;
	clickedNode = NULL;
	connEdit = NULL;
	flagEdit = NULL;
	movingView = false;
	showNodeNames = true;

	for (int i = 0; i < 2; ++i)
		connectingNodes[i] = NULL;

	imageManager.loadImage("assets/images/interface/buttons/flag.png", "flag");
	imageManager.loadImage("assets/images/interface/buttons/plus.png", "plus");

	ibox = InputBox(sf::Vector2f(window.getSize().x - 290,window.getSize().y - 50), sf::Vector2f(280,25), font);
	ibox.setActive(false);

	// Defining the UI colors
	colors["button"] = sf::Color(142, 196, 137);
	colors["background"] = sf::Color(31,31,31);
	colors["graphBG"] = sf::Color(43,43,43);
	colors["buttonHighlight"] = sf::Color(179,80,80);

	moduleName.setString("untitled");
	moduleName.setFont(font);
	moduleName.setCharacterSize(20);
	moduleName.setColor(sf::Color::White);

	sf::Vector2f bgSize(300, window.getSize().y);
	graphBG = sf::RectangleShape(bgSize);
	graphBG.setFillColor(colors["graphBG"]);
	graphBG.setPosition(window.getSize().x - 300,0);
	moduleName.setPosition(graphBG.getPosition());

	// Setting the view
	graphView.reset(sf::FloatRect(0,0,window.getSize().x, window.getSize().y));

	// Initilising the buttons
	buttons["m.name"] = new Button(sf::Vector2f(80,20), colors["button"],2);
	buttons["m.name"]->setPosition(sf::Vector2f(window.getSize().x - 100, 100));

	buttons["n.node"] = new Button(sf::Vector2f(80,20), colors["button"],2);
	buttons["n.node"]->setPosition(sf::Vector2f(window.getSize().x - 290, 100));

	//buttons["n.names"] = new Button(sf::Vector2f(80, 20), colors["button"],2);
	//buttons["n.names"]->setPosition(sf::Vector2f(window.getSize().x - 200, 100));
	buttons["n.ndoe"] = new Button(sf::Vector2f(80, 20), colors["button"],2);
	buttons["n.ndoe"]->setPosition(sf::Vector2f(window.getSize().x - 200, 100));

	buttons["exit"] = new Button(sf::Vector2f(80, 20), colors["button"],2);
	buttons["exit"]->setPosition(sf::Vector2f(window.getSize().x - 100, 50));

	buttons["load"] = new Button(sf::Vector2f(80, 20), colors["button"],2);
	buttons["load"]->setPosition(sf::Vector2f(window.getSize().x - 290, 50));

	buttons["save"] = new Button(sf::Vector2f(80, 20), colors["button"],2);
	buttons["save"]->setPosition(sf::Vector2f(window.getSize().x - 200, 50));

	// Setting the button text
	for(auto i : buttons)
		i.second->setText(i.first, font);

	connSpawn = sf::Vector2f(window.getSize().x - 290, 200);
}

Grapher::~Grapher()
{
	for(auto i : nodeViews)
	{
		delete i;
		i = NULL;
	}

	if (connEdit != NULL)
	{
		delete connEdit;
		connEdit = NULL;
	}

	if (flagEdit != NULL)
	{
		delete flagEdit;
		flagEdit = NULL;
	}
}

void Grapher::run()
{
	window.setFramerateLimit(60);
	inputManager.addBinding("tab_confirm", sf::Keyboard::Tab);

	// main loop
	while(!close)
	{
		update();
		render();
	}
}

void Grapher::update()
{
	sf::Event event;

	// Updating our key input
	inputManager.update(window);
	sf::Vector2f viewPos = window.mapPixelToCoords((sf::Vector2i)inputManager.getMousePosition(), graphView);

	if (inputManager.pressedOnce("cancel"))
	{
		if (selectedInputBox)
		{
			if (ibox.isSelected())
			{
				ibox.setActive(false);
				ibox.setSelected(false);
				ibox.clear();
			}
			else if (selectedNode)
			{
				// Reverting the edited textbox
				selectedInputBox->setString(selectedNode->getUnsavedInput());
			}

			selectedInputBox->setSelected(false);
			selectedInputBox = NULL;
		}


		if (connEdit != NULL)
		{
			if (connEdit->selectionExists())
				connEdit->cancelEdits();
			else
			{
				delete connEdit;
				connEdit = NULL;

				selectedNode = NULL;
			}
		}

		if (flagEdit != NULL)
		{
			delete flagEdit;
			flagEdit = NULL;
		}
	}

	// Tab switching 
	if (inputManager.pressedOnce("tab_confirm") &&
		selectedInputBox != NULL &&
		selectedNode != NULL)
	{
		selectedNode->updateNodeText();
		selectedInputBox->setSelected(false);
		selectedInputBox = selectedNode->getOtherInputBox(selectedInputBox);
		selectedInputBox->setSelected(true);
	}

	while(window.pollEvent(event))
	{
		switch(event.type)
		{
			case sf::Event::Closed:
			  exit(0);
			  break;

			case sf::Event::TextEntered:
				onTextEntered(event.text.unicode);
				break;

			case sf::Event::MouseWheelScrolled:
				if(flagEdit != NULL)
					flagEdit->checkScroll(event.mouseWheelScroll.delta);
				else
					onMouseScroll(event.mouseWheelScroll.delta);

				break;
			
			case sf::Event::MouseButtonReleased:
			{
				if (event.mouseButton.button == sf::Mouse::Left)
					onLeftRelease();
				else if (event.mouseButton.button == sf::Mouse::Middle)
					onMiddleRelease(viewPos);
				break;
			}

			case sf::Event::MouseButtonPressed:
			{
				if(event.mouseButton.button == sf::Mouse::Left)
					onLeftClick(viewPos);
				else if (event.mouseButton.button == sf::Mouse::Right)
					onRightClick(viewPos);
				else if (event.mouseButton.button == sf::Mouse::Middle)
					onMiddleClick(viewPos);
			}
			break;
		}
	}

	// Moving nodes
	if(clickedNode)
	{
		sf::Vector2f moveVec = inputManager.getMousePosition() - inputManager.getPrevMousePosition();
		moveVec *= scale;
		clickedNode->move(moveVec);
	}
	else if(movingView)
	{
		float moveScale = scale - 1.f;

		if (moveScale == 0)
			moveScale = 1.f;
		else if (moveScale < 0)
			moveScale *= -1;

		graphView.move((inputManager.getPrevMousePosition() - inputManager.getMousePosition()) * moveScale);
	}

	for(auto i : buttons)
	{
		i.second->update(&inputManager);

		if(i.second->isMouseOver())
			i.second->setHighlight(colors["buttonHighlight"]);
		else i.second->clearHighlight();
	}

	// Updating NodeView objects
	for (auto i : nodeViews)
	{
		i->update();
		i->updateLines(nodeViews);
	}
}

int Grapher::populateGraph(const std::vector<Node*>& nodes)
{
	if (nodes.size() == 0)
	{
		std::cerr << "no nodes in file!" << std::endl;
		return 1;
	}

	sf::Vector2f spawnPos = window.mapPixelToCoords(sf::Vector2i(50,50), graphView);
	std::list<NodeView*> openSet;
	std::vector<NodeView*> closedSet;

	// Clearing any preexisting nodeViews
	for (auto i = nodeViews.begin(); i != nodeViews.end(); )
	{
		delete *i;
		i = nodeViews.erase(i);
	}

	for (int i = 0; i < 2; ++i)
		connectingNodes[i] = NULL;

	for (auto i : nodes)
	{
		nodeViews.push_back(new NodeView(spawnPos, font, i));
	}
	
	std::cout << "\tpopulated nodeviews" << std::endl;

	for (auto i : nodeViews)
	{
		i->populateLines(nodeViews);
		i->update();
	}

	std::cout << "\tpopulatedLines" << std::endl;

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
		closedSet.push_back(openSet.front());
		openSet.pop_front();
	}

	for (auto i : nodeViews)
		i->updateLines(nodeViews);


	nodeCount = nodeViews.size();
	return 0;
}

void Grapher::render()
{
	window.clear(sf::Color(31,31,31));

	window.setView(graphView);
	for(auto i : nodeViews)
		i->render(window, showNodeNames);

	window.setView(window.getDefaultView());

	window.draw(graphBG);
	window.draw(moduleName);
	ibox.render(window);

	if (connEdit != NULL)
		connEdit->render(window);

	if (flagEdit != NULL)
		flagEdit->render(window);

	for(auto i : buttons)
		i.second->draw(&window);
	window.display();
}

void Grapher::onTextEntered(int unicode)
{
	if (selectedInputBox)
	{
		if (unicode < 128 && unicode > 31)
		{
			selectedInputBox->addCharacter(static_cast<char>(unicode));
		}
		else if (unicode == 8) //Backspace
		{
			selectedInputBox->removeCharacter();
		}
		else if (unicode == 13) //Return
		{
			// If there's an InputBox selected
			if (selectedInputBox)
			{
				// If it's the module's inputbox
				if (ibox.isSelected())
				{
					if (flagEdit != NULL)
						flagEdit->inputString(ibox.getString());

					switch (inState)
					{
						case NAME:
						{
							 moduleName.setString(ibox.getString());

							 int count = 0;
							 for (auto i : nodeViews)
							 {
								 i->setID(moduleName.getString(), count);
								 count++;
							 }
							 break;
						}
						case LOAD:
						{
							 std::string newName;
							 lFlags.clear();
							 sFlags.clear();

							 //if (populateGraph(fileManager.loadDialogue(ibox.getString(), newName)) == 0)
							 if (populateGraph(fileManager.loadDialogue("2calm", newName)) == 0)
							 {
								 moduleName.setString(newName);
								 lFlags = fileManager.loadLocals(newName);
								 sFlags = fileManager.loadShared(newName);
							 }
							 else
								 std::cerr << "file " << ibox.getString() << " contained no nodes!" << std::endl;
							 break;
						}
					}

					// Clearing the box and InputState
					if (flagEdit != NULL)
					{
						if (!flagEdit->gettingText())
						{
							ibox.setSelected(false);
							ibox.setActive(false);
						}
					}
					else
					{
						ibox.setSelected(false);
						ibox.setActive(false);
					}
					ibox.clear();
					inState = NONE;
				}
				else if (connEdit == NULL)// If it's one of the nodeView input boxes
				{
					selectedInputBox->setSelected(false);
					selectedNode->updateNodeText();

					// Deselecting
					selectedNode = NULL;
					selectedInputBox = NULL;

					if (connEdit)
					{
						delete connEdit;
						connEdit = NULL;
					}
				}
			}
		}
	}
	else if (connEdit)
		connEdit->updateText(unicode);
}
void Grapher::onMouseScroll(float delta)
{
	float scaleFactor = 1.0f;
	if (delta < 0)
	{
		scaleFactor = 0.90f;
	}
	else if (delta > 0)
	{
		scaleFactor = 1.10f;
	}

	scale *= scaleFactor;

	graphView.zoom(scaleFactor);
}

void Grapher::onLeftRelease()
{
	clickedNode = NULL;
	movingView = false;

	for (auto i : buttons)
		i.second->setColor(colors["button"]);
}

void Grapher::onMiddleRelease(sf::Vector2f& viewPos)
{
	// If we're making a connection
	if (connectingNodes[0] != NULL)
	{
		for (auto i : nodeViews)
		{
			if (i->getGlobalBounds().contains(viewPos)
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

void Grapher::onLeftClick(sf::Vector2f& viewPos)
{
	if (connEdit == NULL && flagEdit == NULL)
	{
		if (selectedInputBox != NULL)
		{
			for (auto i : nodeViews)
			{
				if (i->getBodyBox() == selectedInputBox || i->getHeaderBox() == selectedInputBox)
					selectedInputBox->setString(i->getUnsavedInput());
			}
		}
		for (auto i : nodeViews)
		{
			selectedInputBox = i->getSelectedInputBox(viewPos);

			if (selectedInputBox != NULL)
			{
				selectedNode = i;
				break;
			}
		}
	}

	// If we've clicked in the graph
	//if (graphBG.getGlobalBounds().contains(inputManager.getMousePosition()))
	const sf::FloatRect& bnds = graphBG.getGlobalBounds();
	sf::Vector2f mspos = inputManager.getMousePosition();
	if (bnds.contains(mspos))
	{
		if (connEdit != NULL)
		{
			int sel = connEdit->updateSelection(inputManager);
			if (sel != -1)
			{
				sf::Vector2f size(window.getSize().x - graphBG.getSize().x, window.getSize().y);

				flagEdit = new FlagEditor(selectedNode->getNode()->getConnections()[sel],
				lFlags, sFlags, size, font, imageManager.getTexture("plus"), fileManager);
			}
		}

		if (ibox.checkClicked(viewPos))
			selectedInputBox = &ibox;
		else
		{
			ibox.clear();
			ibox.setActive(false);
		}
	}
	else
	{
		if (flagEdit == NULL)
		{
			if (selectedInputBox == NULL)
				selectedNode = NULL;

			if (connEdit)
			{
				delete connEdit;
				connEdit = NULL;
			}

			// Selecting a node
			for (auto i : nodeViews)
			{
				if (i->getGlobalBounds().contains(viewPos))
				{
					if (clock.getElapsedTime().asMilliseconds() < 250)
						selectedNode = i;
					else
						clickedNode = i;
				}
			}

			// Node double clicked
			if (clock.getElapsedTime().asMilliseconds() < 250 && selectedNode != NULL)
			{
				if (selectedNode != NULL)
					connEdit = new ConnectionEditor(selectedNode, connSpawn, font, imageManager.getTexture("flag"));
			}
		}
		else
		{
			if (flagEdit->checkButtons(&inputManager))
			{
				selectedInputBox = &ibox;
				ibox.setActive(true);
				ibox.setSelected(true);
			}
			flagEdit->incrementFlags(inputManager.getMousePosition());
		}
	}
	clock.restart();

	if (!clickedNode)
	{
		if (!graphBG.getGlobalBounds().contains(viewPos))
			movingView = true;
	}

	// Checking if any of the buttons have been clicked
	for (auto i : buttons)
	{
		i.second->update(&inputManager);

		if (i.second->isPressed())
		{
			i.second->setColor(colors["background"]);

			if (i.first == "exit")
				exit(0);

			// Flagging the input box as selected if needed
			if (i.first == "m.name")
			{
				ibox.setSelected(true);
				ibox.setActive(true);
				selectedInputBox = &ibox;
				inState = NAME;
			}

			if (i.first == "n.node")
			{
				sf::Vector2f spawnPos = window.mapPixelToCoords(sf::Vector2i(50, 50), graphView);
				nodeViews.push_back(new NodeView(moduleName.getString(),
					nodeCount, spawnPos, font));

				// Tracking the total nodes created to prevent repeated IDs
				nodeCount++;
			}

			if (i.first == "load")
			{
				ibox.setSelected(true);
				ibox.setActive(true);
				selectedInputBox = &ibox;
				inState = LOAD;
			}

			if (i.first == "save")
			{
				std::vector<Node*> nodes;
				for (auto i : nodeViews)
					nodes.push_back(i->getNode());

				fileManager.saveDialogue(moduleName.getString(), nodes);
				fileManager.saveLocals(moduleName.getString(), lFlags);
				fileManager.saveShared(moduleName.getString(), sFlags);
			}

			if (i.first == "n.names")
				showNodeNames = !showNodeNames;
		}

	}
}

void Grapher::onRightClick(sf::Vector2f& viewPos)
{
	if (flagEdit != NULL)
		flagEdit->decrementFlags(inputManager.getMousePosition());
	else
	{
		for (auto i = nodeViews.begin(); i != nodeViews.end();)
		{
			if ((*i)->removeRequired(viewPos))
			{
				// Ensuring that nodes connected to i are disconnected cleanly
				for (auto j : nodeViews)
				{
					if (j != *i)
					{
						j->removeConnector(*i);
					}
				}

				// Clearing selections, just in case
				selectedNode = NULL;
				connectingNodes[0] = NULL;
				connectingNodes[1] = NULL;

				if (selectedInputBox != &ibox)
					selectedInputBox = NULL;

				delete *i;
				*i = NULL;

				i = nodeViews.erase(i);
			}
			else ++i;
		}

		// Repopulating lines
		for (auto i : nodeViews)
			i->populateLines(nodeViews);
	}
}

void Grapher::onMiddleClick(sf::Vector2f& viewPos)
{
	if (connEdit == NULL && flagEdit == NULL)
	{
		connectingNodes[0] = NULL;
		for (auto i : nodeViews)
		{
			if (i->getGlobalBounds().contains(viewPos))
			{
				std::cout << "Node Connection started" << std::endl;
				connectingNodes[0] = i;
				break;
			}
		}

		if (connectingNodes[0] == NULL)
			std::cout << "No nodeview found!" << std::endl;
	}
	else if (flagEdit != NULL)
	{
		flagEdit->removeFlags(inputManager.getMousePosition());
	}
}
