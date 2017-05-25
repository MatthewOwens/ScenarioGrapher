#ifndef FILEMANAGER_H
#define FILEMANAGER_H
#include <json/json.h>
#include <string>
#include <vector>
#include "Node.h"

// Class to parse json dialogue modules and flag files.
class FileManager
{
public:
	FileManager(const std::string& dialogueFolder);
	~FileManager();
	std::vector<Node*> loadDialogue(const std::string& moduleFile);
	std::vector<Node*> loadDialogue(const std::string& moduleFile, std::string& moduleName);
	bool saveDialogue(const std::string& moduleName, std::vector<Node*>);

	//std::map<std::string, int> loadGlobals();
	std::map< std::string, std::map<std::string, int> > loadShared(const std::string& moduleName);
	std::map<std::string, int> loadLocals(const std::string& moduleName);

	//void saveGlobals(const std::map<std::string, int>& map);
	void saveShared(const std::string& moduleName, const std::map< std::string, std::map<std::string, int> >& map);
	void saveLocals(const std::string& moduleName, const std::map<std::string, int>& map);
private:
	std::map<std::string, int> loadFlags(const std::string& filePath);
	std::map<std::string, int> loadFlags(const std::string& filePath, const Json::Value& obj);
	void saveFlags(const std::string& filePath, const std::map<std::string, int>& flags);

	//const std::string globalFlagPath;
	const std::string dialogueFolder;
};

#endif//FILEMANAGER_H
