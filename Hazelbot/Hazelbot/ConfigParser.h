#include <dpp/dpp.h>
#include <sstream>
#include "StringUtils.h"

#ifndef CONFIG_PARSER_H
#define CONFIG_PARSER_H


static class ConfigParser {
public:
	static int get_wordcount();
	static void initialize_configuration();
	static std::string get_string(std::string key);
	static bool get_boolean(std::string key);
	static int get_integer(std::string key);
};

#endif