#include <dpp/dpp.h>
#include <sstream>
#include "StringUtils.h"

#ifndef CONFIG_PARSER_H
#define CONFIG_PARSER_H


static class ConfigParser {
public:
	static int get_wordcount();
	static void initialize_configuration();
	static bool does_key_exist(std::string key);
	static std::string get_string(std::string key, std::string _default);
	static bool get_boolean(std::string key, bool _default);
	static int get_integer(std::string key, int _default);
};

#endif