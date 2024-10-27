#include "ConfigParser.h"
#include "Logger.h"

static std::map<std::string, std::string> config;

int ConfigParser::get_wordcount() {
	return 0;
}

void ConfigParser::initialize_configuration() {
  Log("Initializing Configuration...", DEBUG, "ConfigParser");
	// convert file to string
	std::ifstream file("config/hazelbot.cfg");
	std::stringstream buffer;
	buffer << file.rdbuf();

	// parse string

	// split string at new line
	std::vector<std::string> tokens = StringUtils::split_string_on_nl(buffer.str());

	config = {};

	for (size_t i = 0; i < tokens.size(); i++)
	{
		// splits string into key and value
		std::vector<std::string> t = StringUtils::split_string(tokens[i], '=');

    Log("Found config entry { " + t[0] + ", " + t[1] + " }", DEBUG, "ConfigParser");
		config.insert({t[0], t[1]});
	}
  Log("Configuration Initialized!", INFO, "ConfigParser");
}

bool ConfigParser::does_key_exist(std::string key) {
	if (config.count(key) != 1) {
		return false;
	}
	
	return true;
}

std::string ConfigParser::get_string(std::string key, std::string _default) {
	if (!does_key_exist(key)) {
    Log("Key \"" + key + "\" not found! Defaulting to \"" + _default + "\".", WARNING, "ConfigParser");
		return _default;
	}
	return config[key];
}

int ConfigParser::get_integer(std::string key, int _default) {
	if (!does_key_exist(key)) {
    Log("Key \"" + key + "\" not found! Defaulting to \"" + std::to_string(_default) + "\".", WARNING, "ConfigParser");
		return _default;
	}
	return std::stoi(config[key]);
}

bool ConfigParser::get_boolean(std::string key, bool _default) {
	if (!does_key_exist(key)) {
    Log("Key \"" + key + "\" not found! Defaulting to \"" + std::to_string(_default) + "\".", WARNING, "ConfigParser");
		return _default;
	}

	std::string value = config[key];
	if (StringUtils::to_lower(value) == "true") {
		return true;
	}
	else if (StringUtils::to_lower(value) == "false") {
		return false;
	}

  Log("Failed to parse \"" + key + "\" as boolean.", WARNING, "ConfigParser");
	return _default;
}
