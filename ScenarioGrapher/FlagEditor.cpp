#include "FlagEditor.h"
#include <locale>

FlagEditor::FlagEditor(Connector& connection, std::map<std::string, int>& local,
	std::map<std::string, int>& global, sf::Vector2f availableSize,
	const sf::Font& font, const sf::Texture& buttonTexture)
	:conn(connection),
	localFlags(local),
	globalFlags(global),
	rect(sf::RectangleShape(availableSize)),
	fnt(font)
{
	sf::Vector2f textSpawn = sf::Vector2f(50, 50);
	sf::Vector2f padding = sf::Vector2f(20, 0);

	rect.setPosition(0, 0);
	rect.setFillColor(sf::Color(29,29,29));
	float textHeight = 0.f;
	float textLength = 0.f;
	clickedButton = -1;

	breakTexts[LOCAL] = sf::Text("Initial Local Flags:", font, charSize);
	breakTexts[GLOBAL] = sf::Text("Initial Global Flags:", font, charSize);
	breakTexts[REQUIRED] = sf::Text("Required Flags:", font, charSize);
	breakTexts[TRIGGERED] = sf::Text("Triggered Flags:", font, charSize);

	inStrings[0] = "";
	inStrings[1] = "";

	breakTexts[LOCAL].setPosition(40, 40);

	for (auto i : localFlags)
	{
		localTexts.push_back(sf::Text(i.first, font, charSize));
		localTexts.back().setPosition(textSpawn);

		textHeight = localTexts.back().getGlobalBounds().height;
		textLength = localTexts.back().getGlobalBounds().width;
		
		if (i.second)
			localTexts.push_back(sf::Text("true", font, charSize));
		else
			localTexts.push_back(sf::Text("false", font, charSize));

		localTexts.back().setPosition(textSpawn + padding);
		localTexts.back().move(textLength, 0);

		// Moving the spawn
		textSpawn.y += padding.x + textHeight;
	}

	textSpawn.y += padding.x * 2;
	breakTexts[GLOBAL].setPosition(textSpawn);
	breakTexts[GLOBAL].move(-10, 0);
	textHeight = breakTexts[1].getGlobalBounds().height;
	textSpawn.y += padding.x + textHeight;

	for (auto i : globalFlags)
	{
		globalTexts.push_back(sf::Text(i.first, font, charSize));
		globalTexts.back().setPosition(textSpawn);

		textLength = globalTexts.back().getGlobalBounds().width;
		
		if (i.second)
			globalTexts.push_back(sf::Text("true", font, charSize));
		else
			globalTexts.push_back(sf::Text("false", font, charSize));

		globalTexts.back().setPosition(textSpawn + padding);
		globalTexts.back().move(textLength, 0);

		textSpawn.y += padding.x + textHeight;
	}

	textSpawn.y += padding.x * 2;
	breakTexts[REQUIRED].setPosition(textSpawn);
	breakTexts[REQUIRED].move(-10, 0);
	textHeight = breakTexts[2].getGlobalBounds().height;
	textSpawn.y += padding.x + textHeight;

	for (auto i : conn.getFlags())
	{
		requiredTexts.push_back(sf::Text(i.first, font, charSize));
		requiredTexts.back().setPosition(textSpawn);

		textLength = requiredTexts.back().getGlobalBounds().width;
		
		if (i.second)
			requiredTexts.push_back(sf::Text("true", font, charSize));
		else
			requiredTexts.push_back(sf::Text("false", font, charSize));

		requiredTexts.back().setPosition(textSpawn + padding);
		requiredTexts.back().move(textLength, 0);

		textSpawn.y += padding.x + textHeight;
	}
	
	breakTexts[TRIGGERED].setPosition(textSpawn);
	breakTexts[TRIGGERED].move(-10, 0);
	textHeight = breakTexts[3].getGlobalBounds().height;
	textSpawn.y += padding.x + textHeight;

	for (auto i : conn.getTriggers())
	{
		triggeredTexts.push_back(sf::Text(i.first, font, charSize));
		triggeredTexts.back().setPosition(textSpawn);

		textLength = triggeredTexts.back().getGlobalBounds().width;
		
		if (i.second)
			triggeredTexts.push_back(sf::Text("true", font, charSize));
		else
			triggeredTexts.push_back(sf::Text("false", font, charSize));

		triggeredTexts.back().setPosition(textSpawn + padding);
		triggeredTexts.back().move(textLength, 0);

		textSpawn.y += padding.x + textHeight;
	}

	for (int i = 0; i < 4; ++i)
	{
		buttons[i] = new Button(buttonTexture);
		buttons[i]->setPosition(breakTexts[i].getPosition());
		buttons[i]->move(sf::Vector2f(500, 0));
	}
}
	
void FlagEditor::moveTextBlock(TextBlocks block, float moveVal)
{
	switch (block)
	{
		case LOCAL:
			breakTexts[LOCAL].move(0, moveVal);
			for (auto &i : localTexts)
				i.move(0, moveVal);
			buttons[LOCAL]->move(0, moveVal);
		case GLOBAL:
			breakTexts[GLOBAL].move(0, moveVal);
			for (auto &i : globalTexts)
				i.move(0, moveVal);
			buttons[GLOBAL]->move(0, moveVal);
		case REQUIRED:
			breakTexts[REQUIRED].move(0, moveVal);
			for (auto &i : requiredTexts)
				i.move(0, moveVal);
			buttons[REQUIRED]->move(0, moveVal);
		case TRIGGERED:
			breakTexts[TRIGGERED].move(0, moveVal);
			for (auto &i : triggeredTexts)
				i.move(0, moveVal);
			buttons[TRIGGERED]->move(0, moveVal);
	}
}

// Returns true if a button was pressed, false otherwise
bool FlagEditor::checkButtons(InputManager* inputManager)
{
	clickedButton = -1;
	for (int i = 0; i < 4; ++i)
	{
		buttons[i]->update(inputManager);
		if (buttons[i]->isPressed())
			clickedButton = i;
	}

	return (clickedButton > -1 && clickedButton <= 3);
}

void FlagEditor::incrementFlags(const sf::Vector2f& mousePos)
{
	increment(mousePos, requiredTexts, conn.getFlags());
	increment(mousePos, triggeredTexts, conn.getTriggers(), false);
}

void FlagEditor::decrementFlags(const sf::Vector2f& mousePos)
{
	decrement(mousePos, requiredTexts, conn.getFlags());
	decrement(mousePos, triggeredTexts, conn.getTriggers(), false);
	decrement(mousePos, globalTexts, globalFlags);
	decrement(mousePos, localTexts, localFlags);
}

void FlagEditor::decrement(const sf::Vector2f& mousePos, std::vector<sf::Text>& vec,
						std::map<std::string, int>& map, bool set)
{
	if (map.size() == 0)
		return;

	auto mapItr = map.begin();
	int vecItr = 0;
	bool found = false;

	for (; vecItr < vec.size(); ++vecItr)
	{
		if (vec[vecItr].getGlobalBounds().contains(mousePos))
		{
			found = true;
			break;
		}

		if (vecItr % 2 == 0 && vecItr != 0)
			mapItr++;
	}

	if (found)
	{
		// Decrementing the flag
		mapItr->second--;

		if (vecItr % 2 == 1)
			vecItr--;

		// Updating the string
		if(set)
			vec[vecItr].setString(std::to_string(mapItr->second));
		else
		{
			std::string sign = "";
			if (mapItr->second > 0)
				sign = "+";
			else sign = "-";

			vec[vecItr].setString(sign + std::to_string(mapItr->second));
		}
	}
}

void FlagEditor::increment(const sf::Vector2f& mousePos, std::vector<sf::Text>& vec,
						std::map<std::string, int>& map, bool set)
{
	int vecSel = -1;
	auto mapItr = map.begin();

	for (int i = 0; i < vec.size(); ++i)
	{
		if (vec[i].getGlobalBounds().contains(mousePos))
		{
			vecSel = i;

			if (i % 2 == 0)	// Clicked on the value of the flag
				vecSel = i + 1;

			break;
		}

		if (i % 2 == 0 && i != 0)
			mapItr++;
	}

	if (vecSel != -1)
	{
		mapItr->second++;

		if(set)
			vec[vecSel].setString(std::to_string(mapItr->second));
		else
		{
			std::string sign = "";
			if (mapItr->second > 0)
				sign = "+";
			else sign = "-";

			vec[vecSel].setString(sign + std::to_string(mapItr->second));
		}
	}
}

void FlagEditor::inputString(std::string str)
{
	int flagVal;

	if (inStrings[0] == "")
	{
		inStrings[0] = str;
		return;
	}
	else
	{
		inStrings[1] = str;
		/*std::transform(str.begin(), str.end(), str.begin(), ::tolower);

		if (str == "false")
		{
			inStrings[1] = str;
			flagVal = false;
		}
		else if (str == "true")
		{
			inStrings[1] = str;
			flagVal = true;
		}
		else
		{
			inStrings[0] = "";
			inStrings[1] = "";
			return;
		}*/

		try
		{
			flagVal = std::stoi(inStrings[1]);
		}
		catch (std::exception& e)
		{
			inStrings[0] = "";
			inStrings[1] = "";
			return;
		}
	}
	
	// Both inStrings populated with valid values
	switch ((TextBlocks)clickedButton)
	{
	case REQUIRED:
		addText(requiredTexts);
		conn.addFlag(inStrings[0], flagVal);
		break;
	case TRIGGERED:
		addText(triggeredTexts);
		conn.addTrigger(inStrings[0], flagVal);
		break;
	case GLOBAL:
		addText(globalTexts);
		globalFlags[inStrings[0]] = flagVal;
		break;
	case LOCAL:
		addText(localTexts);
		localFlags[inStrings[0]] = flagVal;
		break;
	}

	moveTextBlock((TextBlocks)(clickedButton + 1), 50.f);

	inStrings[0] = "";
	inStrings[1] = "";
}

void FlagEditor::addText(std::vector<sf::Text>& vec)
{
	auto ref = breakTexts[clickedButton];
	int count = vec.size();
	int widthPadding = 20;

	vec.push_back(sf::Text(inStrings[0], fnt, charSize));
	if (count != 0)
	{
		vec.back().setPosition(vec[count - 2].getPosition());
		vec.back().move(0, vec[count-2].getGlobalBounds().height + 20);
	}
	else
	{
		vec.back().setPosition(ref.getPosition());
		vec.back().move(10, ref.getGlobalBounds().height + 20);
	}

	auto keyItr = vec.back();
	sf::Vector2f keyTextSize(keyItr.getGlobalBounds().width, keyItr.getGlobalBounds().height);

	vec.push_back(sf::Text(inStrings[1], fnt, charSize));
	if (count != 0)
	{
		vec.back().setPosition(vec[count - 2].getPosition());
		vec.back().move(0 + widthPadding + keyTextSize.x, vec[count-2].getGlobalBounds().height + 20);
	}
	else
	{
		vec.back().setPosition(ref.getPosition());
		vec.back().move(10 + widthPadding + keyTextSize.x, ref.getGlobalBounds().height + 20);
	}
}

void FlagEditor::render(sf::RenderWindow& window)
{
	window.draw(rect);

	for (auto i : localTexts)
		window.draw(i);

	for (auto i : globalTexts)
		window.draw(i);

	for (auto i : requiredTexts)
		window.draw(i);

	for (auto i : triggeredTexts)
		window.draw(i);

	for (auto i : breakTexts)
		window.draw(i);

	for (auto i : buttons)
		i->draw(&window);
}

FlagEditor::~FlagEditor()
{
	for (int i = 0; i < 4; ++i)
	{
		delete buttons[i];
		buttons[i] = NULL;
	}
}
