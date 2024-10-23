#pragma once
#include "Common.h"
#include "CountingState.h"

enum CountingFailChainCondition{
  WRONG_NUMBER,
  DOUBLE_SEND
};

class Counting{
private:
  static dpp::snowflake _countingChannelID;
  static void saveState();
  static bool isCountingMessage(const dpp::message_create_t& event);
  static int getFirstNumberInString(const std::string& str);
  static void onFailChain(const dpp::message_create_t& event, const CountingFailChainCondition& condition);
  static void onContinueChain(const dpp::message_create_t& event);
public:
  static void InitializeCounting();
  static void OnMessageCreate(const dpp::message_create_t& event);
  static CountingState CountingState;
};
