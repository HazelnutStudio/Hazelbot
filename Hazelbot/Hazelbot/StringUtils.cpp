#include "StringUtils.h"

std::string StringUtils::to_lower(std::string str) {
	std::transform(str.begin(), str.end(), str.begin(), [](unsigned char c) {return std::tolower(c);});
	return str;
}

bool StringUtils::ends_with(std::string str, std::string ending) {
	// the ending text is larger than the original string.
	if (ending.size() > str.size()) return false;
	return std::equal(ending.rbegin(), ending.rend(), str.rbegin());
}

bool StringUtils::starts_with(std::string str, std::string start) {
	// the starting text is larger than the original string
	if (start.size() > str.size()) return false;
	return std::equal(start.begin(), start.end(), str.begin());
}