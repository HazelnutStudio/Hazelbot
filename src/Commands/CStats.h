#ifndef CSTATS_H
#define CSTATS_H

#include "../Common.h"
#include "../Modules/Counting/Counting.h"
#include <dpp/dispatcher.h>

class CStats{
public:
  void InitializeCommand(const dpp::ready_t& event, Counting* countingInstance);
  void OnCommandRun(const dpp::slashcommand_t& event);
};

#endif
