#ifndef CLIP_H
#define CLIP_H

#include "Common.h"

struct Clip_MessageInfo{
private:
  std::string _messageContent;
  dpp::snowflake _author;
public:
  Clip_MessageInfo(std::string messageContent, dpp::snowflake author);
  std::string GetMessageContent();
  dpp::snowflake GetMessageAuthor();
};

class Clip{
private:
  bool isClipsMessage(dpp::snowflake channelId, dpp::snowflake messageId);
  void addTopClip(Clip_MessageInfo& message, dpp::cluster* bot, std::string key);
  void onMessageReactionAddGetMessageCallback(const dpp::confirmation_callback_t& callback, const dpp::message_reaction_add_t& event);
public:
  std::unordered_map<std::string, Clip_MessageInfo> ActiveVotes;
  void OnMessageSent(const dpp::message_create_t& event);
  void OnMessageReactionAdd(const dpp::message_reaction_add_t& event);
};


#endif
