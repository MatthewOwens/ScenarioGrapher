#include "Connector.h"
#include <iostream>

Connector::Connector(Node* startNode, Node* endNode, int priority)
{
	if (!endNode || !startNode)
	{
		std::cerr << "Error, invalid node connection!" << std::endl;
		exit(1);
	}

	this->startNode = startNode;
	this->endNode = endNode;
	this->priority = priority;
	choiceText = "";
}

Connector::Connector(Node* startNode, Node* endNode, std::string choiceText,
					 int priority)
{
	if (!endNode || !startNode)
	{
		std::cerr << "Error, invalid node connection!" << std::endl;
		exit(1);
	}
	this->startNode = startNode;
	this->endNode = endNode;
	this->choiceText = choiceText;
	this->priority = priority;
}

void Connector::addFlag(std::string flag, bool state)
{
	flags.insert(std::pair<std::string,bool>(flag, state));
}

void Connector::addTrigger(std::string flag, bool state)
{
	triggeredFlags.insert(std::pair<std::string,bool>(flag, state));
}

bool Connector::conditionsMet(std::map<std::string, bool>& localFlags,
							  std::map<std::string, bool>& globalFlags)
{
	std::vector<bool> flagsMet;

	// Checking all the connection's required flags
	for(auto&& flag : flags)
	{
		flagsMet.push_back(false);

		// Giving the flags local to the current module precedence
		for(auto&& local : localFlags)
		{
			if(flag.first == local.first)
			{
				// Checking if the flag was met locally
				flagsMet.back() = (flag.second == local.second);
				break;
			}
		}

		// If the flag wasn't met, check the global flags
		if(!flagsMet.back())
		{
			for(auto&& global : globalFlags)
			{
				if(flag.first == global.first)
				{
					// Checking if the flag was met globally
					flagsMet.back() = (flag.second == global.second);
					break;
				}
			}
		}
	}

	// If any of the conditions wern't met, return false
	for(auto condition : flagsMet)
	{
		if(!condition)
			return false;
	}

	return true;
}

Connector::~Connector()
{
}
