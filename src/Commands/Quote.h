#ifndef QUOTE_H
#define QUOTE_H

#include "../Common.h"
#include "Quote_MessageInfo.h"
#include <dpp/dispatcher.h>

class Quote{
private:
  void addQuoteMessage(Quote_MessageInfo& message, dpp::cluster* bot, std::string key);
  int test;

public:
  std::map<std::string, Quote_MessageInfo> ActiveVotes;
  bool IsQuoteMessage(dpp::snowflake channelId, dpp::snowflake messageId);
  void InitializeCommand(const dpp::ready_t& event);
  void OnCommandRun(const dpp::message_context_menu_t& event);
  void OnMessageReactionAdd(const dpp::message_reaction_add_t& event);
};

#endif
