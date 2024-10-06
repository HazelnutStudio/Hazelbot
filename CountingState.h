#include <dpp/dpp.h>

#ifndef COUNTING_STATE_H
#define COUNTING_STATE_H

struct CountingState{
public:
  int current_number;
  dpp::snowflake last_count_author;
  CountingState(); 
};

#endif
