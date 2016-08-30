/*
 *	Helper class used by Node to represent a connection between
 *	multiple dialogue nodes.
 */

#ifndef CONNECTOR_H
#define CONNECTOR_H
#include <string>
#include <vector>
#include <map>
#include "Node.h"
class Connector
{
	friend class Node;

	public:
		Connector(Node* startNode, Node* endNode, int priority = -1);
		Connector(Node* startNode, Node* endNode, std::string choiceText,
				  int priority = -1);
		~Connector();
		void addFlag(std::string flag);
		void addFlag(std::string flag, int val);
		void addTrigger(std::string flag, int val);
		inline Node* const getEnd() { return endNode; }
		inline const std::string& getChoiceText() { return choiceText; }
		inline void setChoiceText(std::string& str) { choiceText = str; }
		inline int getPriority() { return priority; }
		inline void setPriority(int prio) { priority = prio; }
		inline std::map<std::string, int>& getFlags() { return flags; }
		inline std::map<std::string, int>& getTriggers() { return triggeredFlags; }

	protected:
		bool conditionsMet(std::map<std::string, int>& localFlags,
						   std::map<std::string, int>& globalFlags);
		Node* startNode;
		Node* endNode;
		std::string choiceText;
		std::map<std::string, int> flags;
		std::map<std::string, int> triggeredFlags;
		int priority;	// priority values <0 are auto resolved
};
#endif//CONNECTOR_H
