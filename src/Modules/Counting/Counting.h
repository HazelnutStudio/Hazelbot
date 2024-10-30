#ifndef COUNTING_H
#define COUNTING_H

#include "../../Common.h"
#include "CountingState.h"
#include <regex>
#include "CountingSavesystem.h"

enum CountingFailChainCondition{
  WRONG_NUMBER,
  DOUBLE_SEND
};

class Counting{
private:
  dpp::snowflake _countingChannelID;
  void saveState();
  bool isCountingMessage(const dpp::message_create_t& event);
  int getFirstNumberInString(const std::string& str);
  void onFailChain(const dpp::message_create_t& event, const CountingFailChainCondition& condition);
  void onContinueChain(const dpp::message_create_t& event);
public:
  Counting();
  void OnMessageCreate(const dpp::message_create_t& event);
  CountingState State;
};

#endif
