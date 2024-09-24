#include <dpp/dpp.h>

#ifndef STRING_UTILS_H
#define STRING_UTILS_H

static class StringUtils {
public:
	static std::string to_lower(std::string str);
	static bool ends_with(std::string str, std::string ending);
	static bool starts_with(std::string str, std::string start);
};

#endif