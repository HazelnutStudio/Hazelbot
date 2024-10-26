#ifndef CHAT_INTERACTIONS_H
#define CHAT_INTERACTIONS_H

#include "../Common.h"

class ChatInteractions{
public:
  void OnMessageSent(const dpp::message_create_t& event);
};

#endif
