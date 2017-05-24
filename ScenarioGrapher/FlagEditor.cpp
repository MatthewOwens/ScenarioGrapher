#include "FlagEditor.h"
#include <iostream>
#include <locale>

FlagEditor::FlagEditor(Connector& connection, std::map<std::string, int>& local,
	std::map<std::string, std::map<std::string, int>>& shared,
	sf::Vector2f availableSize, const sf::Font& font,
	const sf::Texture& buttonTexture, FileManager& fmgr)
	:
	conn(connection),
	localFlags(local),
	sharedFlags(shared),
	rect(sf::RectangleShape(availableSize)),
	fnt(font),
	fm(fmgr)
{
	sf::Vector2f textSpawn = sf::Vector2f(50, 50);
	sf::Vector2f padding = sf::Vector2f(20, 0);

	rect.setPosition(0, 0);
	rect.setFillColor(sf::Color(29,29,29));
	float textHeight = 0.f;
	float textLength = 0.f;
	clickedButton = -1;

	breakTexts[LOCAL] = sf::Text("Initial Local Flags:", font, charSize);
	breakTexts[GLOBAL] = sf::Text("Initial Shared Flags:", font, charSize);
	breakTexts[REQUIRED] = sf::Text("Required Flags:", font, charSize);
	breakTexts[TRIGGERED] = sf::Text("Triggered Flags:", font, charSize);

	scrollbox = ScrollableRegion(30,30,920,160,NULL);
	scrollbox.setAnchor(&breakTexts[TRIGGERED]);

	inStrings[0] = "";
	inStrings[1] = "";

	breakTexts[LOCAL].setPosition(40, 40);

	for (auto i : localFlags)
	{
		localTexts.emplace_back(sf::Text(i.first, font, charSize));
		localTexts.back().setPosition(textSpawn);
		scrollbox.addElement(&localTexts.back());

		textHeight = localTexts.back().getGlobalBounds().height;
		textLength = localTexts.back().getGlobalBounds().width;

		localTexts.emplace_back(sf::Text(std::to_string(i.second), font, charSize));
		localTexts.back().setPosition(textSpawn + padding);
		localTexts.back().move(textLength, 0);
		scrollbox.addElement(&localTexts.back());


		// Moving the spawn
		textSpawn.y += padding.x + textHeight;
	}

	textSpawn.y += padding.x * 2;
	breakTexts[GLOBAL].setPosition(textSpawn);
	breakTexts[GLOBAL].move(-10, 0);
	textHeight = breakTexts[1].getGlobalBounds().height;
	textSpawn.y += padding.x + textHeight;

	for (auto i : sharedFlags)
	{
		sharedHeaders.emplace_back(sf::Text(i.first, font, charSize));
		sharedHeaders.back().setPosition(textSpawn);
		sharedHeaders.back().setFillColor(sf::Color(242,223,177));
		scrollbox.addElement(&sharedHeaders.back());

		textSpawn.y += padding.x + textHeight;
		//globalTexts.emplace_back(sf::Text(std::to_string(i.second), font, charSize));

		for (auto j : i.second)
		{
			sharedTexts.emplace_back(sf::Text(j.first, font, charSize));
			sharedTexts.back().setPosition(textSpawn);
			scrollbox.addElement(&sharedTexts.back());

			textLength = sharedTexts.back().getGlobalBounds().width;
			//textSpawn.y += padding.x + textHeight;

			sharedTexts.emplace_back(sf::Text(std::to_string(j.second), font, charSize));
			sharedTexts.back().setPosition(textSpawn + padding);
			sharedTexts.back().move(textLength, 0);
			scrollbox.addElement(&sharedTexts.back());

			textSpawn.y += padding.x + textHeight;
		}
	}

	textSpawn.y += padding.x * 2;
	breakTexts[REQUIRED].setPosition(textSpawn);
	breakTexts[REQUIRED].move(-10, 0);
	textHeight = breakTexts[2].getGlobalBounds().height;
	textSpawn.y += padding.x + textHeight;

	for (auto i : conn.getFlags())
	{
		requiredTexts.emplace_back(sf::Text(i.first, font, charSize));
		requiredTexts.back().setPosition(textSpawn);
		scrollbox.addElement(&requiredTexts.back());

		textLength = requiredTexts.back().getGlobalBounds().width;
		requiredTexts.emplace_back(sf::Text(std::to_string(i.second), font, charSize));

		requiredTexts.back().setPosition(textSpawn + padding);
		requiredTexts.back().move(textLength, 0);
		scrollbox.addElement(&requiredTexts.back());

		textSpawn.y += padding.x + textHeight;
	}
	
	breakTexts[TRIGGERED].setPosition(textSpawn);
	breakTexts[TRIGGERED].move(-10, 0);
	textHeight = breakTexts[3].getGlobalBounds().height;
	textSpawn.y += padding.x + textHeight;

	for (auto i : conn.getTriggers())
	{
		triggeredTexts.emplace_back(sf::Text(i.first, font, charSize));
		triggeredTexts.back().setPosition(textSpawn);

		textLength = triggeredTexts.back().getGlobalBounds().width;
		scrollbox.addElement(&triggeredTexts.back());

		triggeredTexts.emplace_back(sf::Text(std::to_string(i.second), font, charSize));
		triggeredTexts.back().setPosition(textSpawn + padding);
		triggeredTexts.back().move(textLength, 0);

		textSpawn.y += padding.x + textHeight;

		// Updating the scrollbox anchor
		scrollbox.setAnchor(&triggeredTexts.back());
		scrollbox.addElement(&triggeredTexts.back());
	}

	for (int i = 0; i < 4; ++i)
	{
		buttons[i] = new Button(buttonTexture);
		buttons[i]->setPosition(breakTexts[i].getPosition());
		buttons[i]->move(sf::Vector2f(500, 0));


		scrollbox.addElement(&breakTexts[i]);
		scrollbox.addElement(buttons[i]->getTransformable());
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
			for (auto &i : sharedTexts)
				i.move(0, moveVal);
			for (auto &i : sharedHeaders)
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
	//decrement(mousePos, globalTexts, globalFlags);
	//decrement(mousePos, localTexts, localFlags);
}

void FlagEditor::removeFlags(const sf::Vector2f& mousePos)
{
	remove(mousePos, requiredTexts, conn.getFlags());
	remove(mousePos, triggeredTexts, conn.getTriggers());
	//remove(mousePos, sharedTexts, sharedFlags);
	remove(mousePos, localTexts, localFlags);
}

void FlagEditor::decrement(const sf::Vector2f& mousePos, std::list<sf::Text>& list,
						std::map<std::string, int>& map, bool set)
{
	if (map.size() == 0)
		return;

	auto mapItr = map.begin();
	auto itr = list.begin();
	int listItr = 0;
	bool found = false;

	//for (; listItr < list.size(); ++listItr)
	for(; itr != list.end(); ++itr)
	{
		if (itr->getGlobalBounds().contains(mousePos))
		{
			found = true;
			break;
		}

		if (listItr % 2 == 0 && listItr != 0)
			mapItr++;
	}

	if (found)
	{
		// Decrementing the flag
		mapItr->second--;

		if (listItr % 2 == 0)
			listItr++;

		// Updating the string
		if(set)
			itr->setString(std::to_string(mapItr->second));
		else
		{
			std::string sign = "";
			if (mapItr->second > 0)
				sign = "+";

			itr->setString(sign + std::to_string(mapItr->second));
		}
	}
}

void FlagEditor::remove(const sf::Vector2f& mousePos, std::list<sf::Text>& list,
						std::map<std::string, int>& map)
{
	int listSel = -1;
	auto mapItr = map.begin();
	auto itr = list.begin();

	for (int i = 0; i < list.size(); ++i)
	{
		listSel++;
		if (itr->getGlobalBounds().contains(mousePos))
		{
			if (listSel % 2 == 1)	// Clicked on the value of the flag
				listSel--;

		}

		if (listSel % 2 == 0 && listSel != 0)
			mapItr++;

	}

	// If there was a match
	if (itr != list.end())
	{
		map.erase(mapItr);
		auto range_end = itr;
		std::advance(range_end, 2);
		list.erase(itr, range_end);
		//list.erase(list.begin() + listSel, list.begin() + listSel + 2);
	}
}

void FlagEditor::increment(const sf::Vector2f& mousePos, std::list<sf::Text>& list,
						std::map<std::string, int>& map, bool set)
{
	int listSel = -1;
	auto itr = list.begin();
	auto mapItr = map.begin();

	//for (int i = 0; i < list.size(); ++i)
	for(; itr != list.end(); ++itr)
	{
		listSel++;
		if (itr->getGlobalBounds().contains(mousePos))
		{
			if (listSel % 2 == 0)	// Clicked on the name of the flag
			{
				itr++;
				listSel++;
			}

			break;
		}

		if(listSel % 2 == 0 && listSel != 0)
			mapItr++;
	}

	if (itr != list.end())
	{
		mapItr->second++;

		if(set)
			itr->setString(std::to_string(mapItr->second));
		else
		{
			std::string sign = "";
			if (mapItr->second > 0)
				sign = "+";

			itr->setString(sign + std::to_string(mapItr->second));
		}
	}
}

void FlagEditor::inputString(std::string str)
{
	if((TextBlocks)clickedButton == GLOBAL)
	{
		addSharedText(str);
		inStrings[0] = "";
		inStrings[1] = "";
		return;
	}

	int flagVal;

	if (inStrings[0] == "")
	{
		inStrings[0] = str;
		return;
	}
	else
	{
		inStrings[1] = str;
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
		scrollbox.setAnchor(&triggeredTexts.back());
		conn.addTrigger(inStrings[0], flagVal);
		break;
	//case GLOBAL:
		//addText(globalTexts);
		//globalFlags[inStrings[0]] = flagVal;
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

void FlagEditor::addText(std::list<sf::Text>& list)
{
	auto ref = breakTexts[clickedButton];
	int count = list.size();
	int widthPadding = 20;
	auto second_last = -- --list.end();

	list.push_back(sf::Text(inStrings[0], fnt, charSize));
	if (count != 0)
	{
		list.back().setPosition(second_last->getPosition());
		list.back().move(0, second_last->getGlobalBounds().height + 20);
	}
	else
	{
		list.back().setPosition(ref.getPosition());
		list.back().move(10, ref.getGlobalBounds().height + 20);
	}

	auto keyItr = list.back();
	sf::Vector2f keyTextSize(keyItr.getGlobalBounds().width, keyItr.getGlobalBounds().height);

	list.push_back(sf::Text(inStrings[1], fnt, charSize));
	if (count != 0)
	{
		list.back().setPosition(second_last->getPosition());
		list.back().move(0+ widthPadding + keyTextSize.x, second_last->getGlobalBounds().height + 20);
	}
	else
	{
		list.back().setPosition(ref.getPosition());
		list.back().move(10 + widthPadding + keyTextSize.x, ref.getGlobalBounds().height + 20);
	}

	// Adding the text to our scrollbox
	scrollbox.addElement(&list.back());
	scrollbox.addElement(&list.back() - 1);
}

void FlagEditor::addSharedText(const std::string& str)
{
	auto ref = breakTexts[clickedButton];
	auto st_last = -- --sharedTexts.end();
	int widthPadding = 20;
	bool usingRef = (sharedHeaders.size() == 0 ? true : false);
	float moveVal = 0.f;

	std::map<std::string, int> newShared;
	newShared = fm.loadLocals(str);

	if(newShared.empty())
		return;

	sharedFlags.insert(std::make_pair(str, newShared));

	// Adding the header text for the shared flags block
	sharedHeaders.emplace_back(sf::Text(str, fnt, charSize));
	sharedHeaders.back().setFillColor(sf::Color(242,223,177));
	moveVal += sharedHeaders.back().getGlobalBounds().height + 20;
	scrollbox.addElement(&sharedHeaders.back());

	if(usingRef)
	{
		sharedHeaders.back().setPosition(ref.getPosition());
		sharedHeaders.back().move(0, ref.getGlobalBounds().height + 20);
	}
	else
	{
		sharedHeaders.back().setPosition(st_last->getPosition());
		sharedHeaders.back().move(0, st_last->getGlobalBounds().height + 20);
	}

	// Moving the header
	ref = sharedHeaders.back();

	for(auto i : newShared)
	{
		// Flag Text
		sharedTexts.emplace_back(sf::Text(i.first, fnt, charSize));
		sharedTexts.back().setPosition(ref.getPosition());
		sharedTexts.back().move(0, ref.getGlobalBounds().height + 20);
		scrollbox.addElement(&sharedTexts.back());

		moveVal += sharedTexts.back().getGlobalBounds().height + 20;

		// Flag Value
		sf::Vector2f keyTextSize(sharedTexts.back().getGlobalBounds().width,
								sharedTexts.back().getGlobalBounds().height);

		sharedTexts.emplace_back(sf::Text(std::to_string(i.second), fnt, charSize));
		sharedTexts.back().setPosition(ref.getPosition());
		sharedTexts.back().move(widthPadding + keyTextSize.x, ref.getGlobalBounds().height + 20);
		scrollbox.addElement(&sharedTexts.back());

		ref = *(-- --sharedTexts.end());
	}

	moveTextBlock(REQUIRED, moveVal);
}

void FlagEditor::render(sf::RenderWindow& window)
{
	window.draw(rect);
	window.draw(scrollbox.getVisBounds());

	for (auto i : localTexts)
		window.draw(i);

	for (auto i : sharedTexts)
		window.draw(i);

	for (auto i : sharedHeaders)
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

void FlagEditor::checkScroll(float delta) { scrollbox.scroll(delta); }

FlagEditor::~FlagEditor()
{
	for (int i = 0; i < 4; ++i)
	{
		delete buttons[i];
		buttons[i] = NULL;
	}
}
