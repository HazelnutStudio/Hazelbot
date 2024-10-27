#ifndef EMOJI_HANDLER_H
#define EMOJI_HANDLER_H

#include "dpp/dpp.h"
#include "ConfigParser.h"

std::string GetEmoji(std::string name, bool inText = false, std::string defaultEmoji = u8"⭐");

#endif
