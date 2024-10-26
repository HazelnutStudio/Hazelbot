#ifndef BOT_RESPONSES_H
#define BOT_RESPONSES_H

#include <string> 
#include <vector>
#include <unordered_map>
#include "StringUtils.h"

void InitializeResponses();
std::string GetResponse(std::string responseId);

#endif
