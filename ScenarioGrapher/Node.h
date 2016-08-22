#ifndef NODE_H
#define NODE_H
#include <string>
#include <vector>
#include <map>
#include "Connector.h"
class Node
{
	public:
		Node(std::string identifier);
		~Node();

		void addConnector(Connector outgoing);
		void setHeader(std::string header);
		void setBody(std::string body);
		void setID(const std::string& id);

		std::string* getChosenConnectorText();

		bool resolveable(std::map<std::string, bool> localFlags,
						 std::map<std::string, bool> globalFlags);

		const std::string& getIdentifier() { return identifier; }
		const std::string& getHeader() { return header; }
		const std::string& getBody() { return body; }
		std::vector<Connector>& getConnections() { return connections; }
	protected:
		std::string identifier;	// Unique to this node in all modules
		std::string header;		// Usually a character's name
		std::string body;		// The dialogue text itself

		std::vector<Connector> connections;
		std::string* chosenConnectorText;
};
#endif//NODE_H
