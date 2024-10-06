#include <dpp/dpp.h>

#ifndef COUNTING_STATE_H
#define COUNTING_STATE_H

struct CountingState{
public:
  int current_number;
  dpp::snowflake last_count_author;
  int highest_count;
  int total_counts;
  dpp::snowflake longest_chain_failed_by;
  int total_failures;
  CountingState(); 
};

#endif
