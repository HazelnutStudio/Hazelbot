#ifndef WISDOM_H
#define WISDOM_H

#include "../Common.h"

class Wisdom{
public:
  void InitializeCommand(const dpp::ready_t& event); 
  void OnCommandRun(const dpp::slashcommand_t& event);
};

#endif
