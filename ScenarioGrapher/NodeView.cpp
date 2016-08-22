#include "NodeView.h"
#include <iostream>

// Determines the node ID using the current module ID and the number of
// nodes in the module
NodeView::NodeView(const std::string& moduleID, int nodeCount,
				   const sf::Vector2f position, sf::Font& font)
{
	node = new Node(moduleID + std::to_string(nodeCount));
	this->position = position;
	this->font = &font;

	// Default sizes
	size = sf::Vector2f(100, 100);
	sf::Vector2f iboxSize(size.x - (2 * spacing), size.y / 2 - (2*spacing));
	sf::Vector2f iboxPos = this->position;

	// Initilising the base rectangle
	baseRect = sf::RectangleShape(size);
	baseRect.setPosition(this->position);
	baseRect.setPosition(this->position.x, this->position.y);
	baseRect.setFillColor(sf::Color(100, 32, 32));
	idText = sf::Text(moduleID + std::to_string(nodeCount), font);
	idText.setPosition(this->position);
	idText.move(0, textSpacingY);

	// Initilising the inlet & outlets
	for (int i = 0; i < 2; ++i)
	{
		circles[i] = sf::CircleShape(circleSize);
		circles[i].setFillColor(sf::Color::Magenta);
		circles[i].setPosition(this->position);
	}
	circles[0].move(-circleSize, size.y / 2 - circleSize / 2);
	circles[1].move(size.x, size.y / 2 - circleSize / 2);

	// Creating the header input box
	iboxPos.x += spacing;
	iboxPos.y += spacing;
	headerInput = InputBox(iboxPos, iboxSize, font);

	// Creating the body input box
	iboxPos = headerInput.getPosition();
	iboxPos.y += (iboxSize.y + spacing * 2);
	bodyInput = InputBox(iboxPos, iboxSize, font);

	lines = sf::VertexArray(sf::Lines, 0);
}

NodeView::NodeView(const sf::Vector2f position, sf::Font& font,
				   Node* node)
{
	this->position = position;
	this->node = node;
	this->font = &font;

	// Default sizes
	size = sf::Vector2f(100, 100);
	sf::Vector2f iboxSize(size.x - (2 * spacing), size.y / 2 - (2*spacing));
	sf::Vector2f iboxPos = this->position;

	// Initilising the base rectangle
	baseRect = sf::RectangleShape(size);
	baseRect.setPosition(this->position);
	baseRect.setPosition(this->position.x, this->position.y);
	baseRect.setFillColor(sf::Color(100, 32, 32));
	idText = sf::Text(node->getIdentifier(), font);
	idText.setPosition(this->position);
	idText.move(0, textSpacingY);

	// Initilising the inlet & outlets
	for (int i = 0; i < 2; ++i)
	{
		circles[i] = sf::CircleShape(circleSize);
		circles[i].setFillColor(sf::Color::Magenta);
		circles[i].setPosition(this->position);
	}
	circles[0].move(-circleSize, size.y / 2 - circleSize / 2);
	circles[1].move(size.x, size.y / 2 - circleSize / 2);

	// Creating the header input box
	iboxPos.x += spacing;
	iboxPos.y += spacing;
	headerInput = InputBox(iboxPos, iboxSize, font);

	// Creating the body input box
	iboxPos = headerInput.getPosition();
	iboxPos.y += (iboxSize.y + spacing * 2);
	bodyInput = InputBox(iboxPos, iboxSize, font);

	lines = sf::VertexArray(sf::Lines, 0);

	// Setting our strings
	headerInput.setString(node->getHeader());
	bodyInput.setString(node->getBody());
	idText.setString(node->getIdentifier());
}

void NodeView::setID(const std::string& moduleID, int nodeNumber)
{
	if (node)
	{
		std::string newID = moduleID + std::to_string(nodeNumber);
		node->setID(newID);
		idText.setString(newID);
	}
}

void NodeView::move(const sf::Vector2f& vector)
{
	baseRect.move(vector);
	idText.move(vector);
	headerInput.move(vector);
	bodyInput.move(vector);

	for (int i = 0; i < 2; ++i)
		circles[i].move(vector);
}

void NodeView::updateLines(const std::list<NodeView*>& nodeViews)
{
	if (lines.getVertexCount() == 0)
		return;

	std::vector<Connector> conns = node->getConnections();

	for (int i = 0; i < lines.getVertexCount(); ++i)
	{
		if (i % 2 == 0)	// line start point
			lines[i].position = circles[1].getPosition();
		else
		{
			Node* target = conns[i / 2].getEnd();
			for (auto j : nodeViews)
			{
				if (j->getNode() == target)
				{
					lines[i].position = j->getInletPos();
					break;
				}
			}

			sf::Vector2f lineVec = lines[i].position - lines[i - 1].position;
			sf::Vector2f lineVecPerp = sf::Vector2f(-lineVec.y, lineVec.x);
			float perpLength = std::sqrt((lineVecPerp.x * lineVecPerp.x) + (lineVecPerp.y * lineVecPerp.y));

			// Converting lineVecPerp to a unit vector
			lineVecPerp /= perpLength;

			// Mid point of lineVec
			sf::Vector2f mid = (lines[i].position + lines[i - 1].position) / 2.f;

			connectorTexts[i/2].setRotation(std::atan2(lineVec.y, lineVec.x) * 180/3.14152);
			connectorTexts[i/2].setPosition(mid + lineVecPerp * 20.f);

			connectorPriorities[i/2].setRotation(std::atan2(lineVec.y, lineVec.x) * 180/3.14152);
			connectorPriorities[i/2].setPosition(mid - lineVecPerp * 20.f);
		}
	}
}

void NodeView::populateLines(const std::list<NodeView*>& nodeViews)
{
	std::vector<Connector> conns = node->getConnections();
	lines.clear();
	connectorTexts.clear();
	connectorPriorities.clear();

	for (auto i : conns)
	{
		Node* target = i.getEnd();
		for (auto j : nodeViews)
		{
			if (j->getNode() == target)
			{
				lines.append(circles[0].getPosition());
				lines[lines.getVertexCount() - 1].color = sf::Color::Blue;

				lines.append(j->getInletPos());
				lines[lines.getVertexCount() - 1].color = sf::Color::Cyan;

				connectorTexts.push_back(sf::Text(i.getChoiceText(), *font, 12));
				connectorPriorities.push_back(sf::Text(std::to_string(i.getPriority()), *font, 12));
				break;
			}
		}
	}
}

sf::FloatRect NodeView::getGlobalBounds()
{
	return baseRect.getGlobalBounds();
}

const std::string& NodeView::getID()
{
	if (node)
		return node->getIdentifier();
	else return "";
}

const sf::Vector2f& NodeView::getPosition()
{
	return baseRect.getPosition();
}

bool NodeView::removeRequired(const sf::Vector2f& mousePos)
{
	if(baseRect.getGlobalBounds().contains(mousePos))
	{
		return true;
	}
	else
	{
		return false;
	}
}
InputBox* NodeView::getSelectedInputBox(const sf::Vector2f& mousePos)
{
	InputBox* ret = NULL;
	if (headerInput.checkClicked(mousePos))
		ret = &headerInput;
	if (bodyInput.checkClicked(mousePos))
		ret = &bodyInput;

	return ret;
}

// Returns the header/body value saved in Node if either input box is selected
std::string NodeView::getUnsavedInput()
{
	if (node == NULL)
		return "";

	if (headerInput.isSelected())
		return node->getHeader();
	else if (bodyInput.isSelected())
		return node->getBody();
	else return "";
}
void NodeView::setPosition(const sf::Vector2f& newPos)
{
	sf::Vector2f moveVec = newPos - position;
	move(moveVec);
	//baseRect.setPosition(newPos);
}

void NodeView::update()
{
	float maxTextWidth;

	// Finding the max text width
	if (headerInput.getString().length() > bodyInput.getString().length())
		maxTextWidth = headerInput.getTextWidth();
	else
		maxTextWidth = bodyInput.getTextWidth();

	// Ensuring that there's always space for one more character
	maxTextWidth += headerInput.getCharacterSize();

	// Do we need to resize?
	if (maxTextWidth < baseRect.getSize().x)
	{
		baseRect.setSize(sf::Vector2f(100 + spacing, baseRect.getSize().y));
		headerInput.setSize(sf::Vector2f(100 - spacing, headerInput.getSize().y));
		bodyInput.setSize(sf::Vector2f(100 - spacing, bodyInput.getSize().y));
		return;
	}

	// Resizing
	baseRect.setSize(sf::Vector2f(maxTextWidth, baseRect.getSize().y));
	headerInput.setSize(sf::Vector2f(maxTextWidth, headerInput.getSize().y));
	bodyInput.setSize(sf::Vector2f(maxTextWidth, bodyInput.getSize().y));

	const sf::FloatRect& rect = baseRect.getGlobalBounds();
	circles[1].setPosition(sf::Vector2f(rect.left + rect.width, rect.top + rect.height / 2));
}

void NodeView::updateNodeText()
{
	if (node)
	{
		node->setHeader(headerInput.getString());
		node->setBody(bodyInput.getString());
	}
}

bool NodeView::addConnector(Connector& connector, const sf::Vector2f& lineTarget)
{
	if (node == NULL)
		return false;
	else
	{
		node->addConnector(connector);
		lines.append(circles[1].getPosition());
		lines[lines.getVertexCount() - 1].color = sf::Color::Blue;

		lines.append(lineTarget);
		lines[lines.getVertexCount() - 1].color = sf::Color::Cyan;

		connectorTexts.push_back(sf::Text(connector.getChoiceText(), *font, 12));
		connectorPriorities.push_back(sf::Text(std::to_string(connector.getPriority()), *font, 12));
		return true;
	}
}

void NodeView::removeConnector(NodeView* target)
{
	Node* targetNode = target->getNode();
	bool connectorRemoved = false;

	std::vector<Connector>& conns = node->getConnections();
	for (auto i = conns.begin(); i != conns.end();)
	{
		if (i->getEnd() == targetNode)
		{
			i = conns.erase(i);
			connectorRemoved = true;
			break; // Target erased, no need to continue
		}
		else ++i;
	}

	// Removing errant lines if we need to
	if (!connectorRemoved)
		return;

	int toRemove = 0;
	sf::VertexArray tmpLines = lines;

	for (; toRemove < lines.getVertexCount() - 1; toRemove++)	// Finding the vertices we need to remove
	{
		if (lines[toRemove].position == target->getInletPos())
			break;
	}

	lines.clear();
	for (int i = 0; i < tmpLines.getVertexCount() - 1; ++i)
	{
		if (i != toRemove && i != toRemove - 1)
			lines.append(tmpLines[i]);
	}
}

InputBox* NodeView::getOtherInputBox(InputBox* selected)
{
	// Checking to ensure that selected is one of this instance's
	// InputBox's
	if (selected != &headerInput && selected != &bodyInput)
		return selected;	// Ensuring that the pointer is unchanged
	else if (selected == &headerInput)
		return &bodyInput;
	else return &headerInput;
}


void NodeView::render(sf::RenderWindow& window, bool showNames)
{
	for (int i = 0; i < 2; ++i)
		window.draw(circles[i]);

	window.draw(baseRect);

	if (showNames)
		window.draw(idText);

	headerInput.render(window);
	bodyInput.render(window);
	window.draw(lines);

	for (int i = 0; i < connectorPriorities.size(); ++i)
	{
		window.draw(connectorPriorities[i]);
		window.draw(connectorTexts[i]);
	}
}

NodeView::~NodeView()
{
	delete node;
	node = NULL;
}