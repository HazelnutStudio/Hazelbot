#include <dpp/dpp.h>

#ifndef COUNTING_STATUS_H
#define COUNTING_STATUS_H

class CountingStatus{
public:
  int current_number;
  dpp::snowflake last_count_author;
  CountingStatus(); 
};

#endif
