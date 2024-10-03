#include <dpp/dpp.h>

#ifndef STRING_UTILS_H
#define STRING_UTILS_H

class StringUtils {
public:
	static std::string to_lower(std::string str);
	static bool ends_with(std::string str, std::string ending);
	static bool starts_with(std::string str, std::string start);
	static std::vector<std::string> split_string(std::string str, char delimiter);
	static std::vector<std::string> split_string_on_nl(std::string str);
};

#endif
