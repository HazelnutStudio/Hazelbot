#include "ConfigParser.h"

static std::map<std::string, std::string> config;

int ConfigParser::get_wordcount() {
	return 0;
}

void ConfigParser::initialize_configuration() {
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

		config.insert({t[0], t[1]});
	}
}

bool ConfigParser::does_key_exist(std::string key) {
	if (config.count(key) != 1) {
		return false;
	}
	
	return true;
}

std::string ConfigParser::get_string(std::string key, std::string _default) {
	if (!does_key_exist(key)) {
		std::cout << "Key \"" << key << "\" not found! defaulting to \"" << _default << "\"." << std::endl;
		return _default;
	}
	return config[key];
}

int ConfigParser::get_integer(std::string key, int _default) {
	if (!does_key_exist(key)) {
		std::cout << "Key \"" << key << "\" not found! defaulting to \"" << std::to_string(_default) << "\"." << std::endl;
		return _default;
	}
	return std::stoi(config[key]);
}

bool ConfigParser::get_boolean(std::string key, bool _default) {
	if (!does_key_exist(key)) {
		std::cout << "Key \"" << key << "\" not found! defaulting to \"" << std::to_string(_default) << "\"." << std::endl;
		return _default;
	}

	std::string value = config[key];
	if (StringUtils::to_lower(value) == "true") {
		return true;
	}
	else if (StringUtils::to_lower(value) == "false") {
		return false;
	}

	std::cout << "Warning: failed to parse \"" << key << "\" as boolean." << std::endl;
	return _default;
}
