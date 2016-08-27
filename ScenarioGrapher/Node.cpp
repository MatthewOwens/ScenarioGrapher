#include "Node.h"
#include <iostream>

Node::Node(std::string identifier)
{
	this->identifier = identifier;
	this->header = "";
	this->body = "";
	this->chosenConnectorText = NULL;
}

Node::~Node()
{
}

void Node::setID(const std::string& id)
{
	identifier = id;

	// TODO: backprop. to update connector IDs
}

// Checks if we can resolve the node without player intervention
bool Node::resolveable(std::map<std::string, int> localFlags,
			  		   std::map<std::string, int> globalFlags)
{
	// If this->is the end of the module, return true
	if(connections.size() == 0)
		return true;

	/* Since the majority of nodes represent unconditional character
	 * switches, checking for a single valid connection here saves some
	 * cycles
	 */
	if(connections.size() == 1
	   && connections.back().conditionsMet(localFlags, globalFlags)
	   && connections.back().priority < 0)
		return true;

	// If any of our conditions explicitly require user interaction
	// return false
	for(auto&& connection : connections)
	{
		if(connection.priority == 0)
			return false;
	}

	// If there's muliple connectors that don't require interaction
	for(auto&& connection : connections)
	{
		if(!connection.conditionsMet(localFlags, globalFlags))
			return false;
	}

	return true;
}

void Node::addConnector(Connector outgoing)
{
	connections.push_back(outgoing);
}

void Node::setHeader(std::string header)
{
	this->header = header;
}

void Node::setBody(std::string body)
{
	this->body = body;
}

std::string* Node::getChosenConnectorText()
{
	return chosenConnectorText;
}
