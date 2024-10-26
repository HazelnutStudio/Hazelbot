#include "Quote_MessageInfo.h"

Quote_MessageInfo::Quote_MessageInfo(dpp::snowflake guildId, dpp::snowflake message, dpp::snowflake messageAuthor, std::string messageContent, std::time_t messageSent){
  _guildId = guildId;
  _message = message;
  _messageAuthor = messageAuthor;
  _messageContent = messageContent;
  _messageSent = messageSent; 
}

Quote_MessageInfo::Quote_MessageInfo(){
  _guildId = 0; 
  _message = 0;
  _messageAuthor = 0;
  _messageContent = ":3";
  _messageSent = 0;
}

dpp::snowflake Quote_MessageInfo::GetGuildId(){
  return _guildId;
}

dpp::snowflake Quote_MessageInfo::GetMessageId(){
  return _message;
}

std::string Quote_MessageInfo::GetMessageContent(){
  return _messageContent;
}

std::time_t Quote_MessageInfo::GetMessageSent(){
  return _messageSent;
}

dpp::snowflake Quote_MessageInfo::GetMessageAuthorId(){
  return _messageAuthor;
}
