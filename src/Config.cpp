#include "Config.hpp"

Config::Config() {}

Config & Config::getInstance() {
	static Config instance;
	return instance;
}

void Config::addKeyValuePair(std::string key, std::string value) {
	config.insert({key, value});
}

void Config::readFromConfigFile(std::string configPath) {
	ConfigParser * parser = new ConfigParser(configPath);
	parser->parseKeyValuePairs();
	delete parser;
}

std::string Config::getValueFromKey(std::string key) {
	return config[key];
}

ConfigParser::ConfigParser(std::string configPath) {
	config.open(configPath, std::ifstream::in);
}

ConfigParser::~ConfigParser() {
	config.close();
}

void ConfigParser::parseKeyValuePairs() {
	std::string line;
	while(std::getline(config, line)) {
		parseSingleLine(line);
	}
}

void ConfigParser::parseSingleLine(std::string line) {
	std::istringstream lineStream(line);
	std::string key;
	if(std::getline(lineStream, key, '=')) {
		std::string value;
		if(std::getline(lineStream, value)) {
			passPairToConfig(key, value);
		}
	}
}

void ConfigParser::passPairToConfig(std::string key, std::string value) {
	Config & instance = Config::getInstance();
	std::string trimmedKey = trimWhitespace(key);
	std::string trimmedValue = trimWhitespace(value);
	instance.addKeyValuePair(trimmedKey, trimmedValue);
}

std::string ConfigParser::trimWhitespace(std::string str) {
	size_t first = str.find_first_not_of(' ');
	if(std::string::npos == first) {
		return str;
	}

	size_t last = str.find_last_not_of(' ');

	return str.substr(first, (last - first + 1));
}
