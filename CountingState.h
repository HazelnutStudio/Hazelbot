#include <dpp/dpp.h>

#ifndef COUNTING_STATE_H
#define COUNTING_STATE_H

struct CountingUserState{
public:
  int highest_count;
  int total_counts;
  int biggest_failure;
  int total_failures;
  CountingUserState();
};

struct CountingState{
public:
  int current_number;
  dpp::snowflake last_count_author;
  int highest_count;
  int total_counts;
  dpp::snowflake longest_chain_failed_by;
  int total_failures;
  std::map<std::string, CountingUserState> user_stats;
  CountingState(); 
};

#endif
