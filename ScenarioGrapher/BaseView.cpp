#include "BaseView.h"
#include "Connector.h"

void BaseView::addConnector(Connector& connector, const sf::Vector2f& lineTarget)
{
	Node::addConnector(connector);
	lines.append(circles[1].getPosition());
	lines[lines.getVertexCount() - 1].color = sf::Color::Blue;

	lines.append(lineTarget);
	lines[lines.getVertexCount() - 1].color = sf::Color::Cyan;

	connectorTexts.push_back(sf::Text(connector.getChoiceText(), *font, 12));
	connectorPriorities.push_back(sf::Text(std::to_string(connector.getPriority()), *font, 12));
}

void BaseView::removeConnector(BaseView* target)
{
	Node* targetNode = target;
	bool connectorRemoved = false;

	for (auto i = connections.begin(); i != connections.end();)
	{
		if (i->getEnd() == targetNode)
		{
			i = connections.erase(i);
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

void BaseView::update()
{
}
