#pragma once

#include <fstream>
#include <sstream>
#include <unordered_map>

// Singleton object
class Config {

friend class ConfigParser;

private:
	std::unordered_map<std::string, std::string> config;

	Config();
	void addKeyValuePair(std::string key, std::string value);

public:
	static Config & getInstance();
	Config(Config const &) = delete;
	void operator=(Config const &) = delete;

	void readFromConfigFile(std::string configPath);
	std::string getValueFromKey(std::string key);

};

class ConfigParser {

private:
	std::ifstream config;

	void parseSingleLine(std::string line);
	void passPairToConfig(std::string key, std::string value);
	std::string trimWhitespace(std::string str);

public:
	ConfigParser(std::string configPath);
	~ConfigParser();
	void parseKeyValuePairs();

};
