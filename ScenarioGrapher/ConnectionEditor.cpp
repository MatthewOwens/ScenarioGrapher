#include "ConnectionEditor.h"

ConnectionEditor::ConnectionEditor(NodeView* target, sf::Vector2f spawnPos, const sf::Font& font, const sf::Texture& flagTexture)
:conns(target->getNode()->getConnections()),
 prioTexts(target->getPrioTexts()),
 choiceTexts(target->getChoiceTexts())
{
	sf::Vector2f stringBoxSize = sf::Vector2f(215, 25);
	sf::Vector2f prioBoxSize = sf::Vector2f(25, 25);
	sf::Vector2f paddingSize = sf::Vector2f(10, 10);
	sf::Vector2f flagOffset = sf::Vector2f(30, 0);

	for (auto i : conns)
	{
		sf::Vector2f prioSpawnPos = spawnPos;
		prioSpawnPos.x += (stringBoxSize.x + paddingSize.x + flagOffset.x);

		// string text box
		iboxes.push_back(InputBox(spawnPos + flagOffset, stringBoxSize, font));
		iboxes.back().setString(i.getChoiceText());

		flagButtons.push_back(new Button(flagTexture));
		flagButtons.back()->setPosition(spawnPos);
		
		// priority text box
		iboxes.push_back(InputBox(prioSpawnPos, prioBoxSize, font));
		iboxes.back().setString(std::to_string(i.getPriority()));

		spawnPos.y += (paddingSize.y + stringBoxSize.y);
	}
	selectedIndex = -1;
}

void ConnectionEditor::updateText(int unicode)
{
	if (unicode < 128 && unicode > 31)
		addCharacter(static_cast<char>(unicode));
	else if (unicode == 8) // backspace
		removeCharacter();
	else if (unicode == 13) // return
		confirmEdits();
}

void ConnectionEditor::addCharacter(char in)
{
	if (selectedIndex == -1)
		return;
	else iboxes[selectedIndex].addCharacter(in);
	std::cout << "iboxes[" << selectedIndex << "] : " << iboxes[selectedIndex].getString() << std::endl;
}

void ConnectionEditor::removeCharacter()
{
	if (selectedIndex == -1)
		return;
	else iboxes[selectedIndex].removeCharacter();
}

void ConnectionEditor::confirmEdits()
{
	if (selectedIndex == -1)
		return;
	else
	{
		if (selectedIndex % 2 == 0)	// string text box
		{
			conns[selectedIndex / 2].setChoiceText(iboxes[selectedIndex].getString());
			choiceTexts[selectedIndex / 2].setString(iboxes[selectedIndex].getString());
		}
		else	// priority text box
		{
			/*if (iboxes[selectedIndex].getString().length() == 0)
				iboxes[selectedIndex].setString("-1");*/
			try
			{
				conns[selectedIndex / 2].setPriority(std::stoi(iboxes[selectedIndex].getString()));
				prioTexts[selectedIndex / 2].setString(iboxes[selectedIndex].getString());
			}
			catch (...)
			{
				std::cerr << "Invalid argument - please enter a number" << std::endl;
				iboxes[selectedIndex].setString(std::to_string(conns[selectedIndex / 2].getPriority()));
			}
		}

		iboxes[selectedIndex].setSelected(false);
	}

	selectedIndex = -1;
}

void ConnectionEditor::cancelEdits()
{
	if (selectedIndex == -1)
		return;
	else
	{
		if (selectedIndex % 2 == 0)	// string text box
			iboxes[selectedIndex].setString(conns[selectedIndex / 2].getChoiceText());
		else	// priority text box
			iboxes[selectedIndex].setString(std::to_string(conns[selectedIndex / 2].getPriority()));

		iboxes[selectedIndex].setSelected(false);
	}

	selectedIndex = -1;
}

int ConnectionEditor::updateSelection(InputManager& inputManager)
{
	//selectedIndex = -1;
	bool selected = false;
	const sf::Vector2f& mousePos = inputManager.getMousePosition();

	for (int i = 0; i < iboxes.size(); ++i)
	{
		if (iboxes[i].checkClicked(mousePos))
		{
			if (selectedIndex != -1)
				confirmEdits();

			selectedIndex = i;
			selected = true;
		}
	}

	if (!selected)
	{
		if (selectedIndex != -1)
			cancelEdits();

		selectedIndex = -1;
	}

	int ret = -1;
	for (int i = 0; i< flagButtons.size(); ++i)
	{
		flagButtons[i]->update(&inputManager);

		if (flagButtons[i]->isPressed())
		{
			flagButtons[i]->setHighlight(sf::Color::Red);
			ret = i;
		}
		else flagButtons[i]->clearHighlight();
	}

	return ret;
}

void ConnectionEditor::render(sf::RenderWindow& window)
{
	for (int i = 0; i < iboxes.size(); ++i)
		iboxes[i].render(window);

	for (auto i : flagButtons)
		i->draw(&window);
}

ConnectionEditor::~ConnectionEditor()
{
	for (int i = 0; i < flagButtons.size(); ++i)
	{
		delete flagButtons[i];
		flagButtons[i] = 0;
	}
}
