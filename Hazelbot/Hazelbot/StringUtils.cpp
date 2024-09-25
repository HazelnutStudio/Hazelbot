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

std::vector<std::string> StringUtils::split_string(std::string str, char delimiter) {
	std::string buffer;
	std::stringstream stream(str);

	std::vector<std::string> tokens;

	// splits string at delimiter
	while (std::getline(stream, buffer, delimiter)) {
		tokens.push_back(buffer);
	}

	return tokens;
}

std::vector<std::string> StringUtils::split_string_on_nl(std::string str) {
	std::string buffer;
	std::stringstream stream(str);

	std::vector<std::string> tokens;

	while (std::getline(stream, buffer)) {
		tokens.push_back(buffer);
	}

	return tokens;
}