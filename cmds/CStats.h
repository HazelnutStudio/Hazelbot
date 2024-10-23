#ifndef CSTATS_H
#define CSTATS_H

#include "../Common.h"
#include "../Counting.h"
#include <dpp/dispatcher.h>

class CStats{
public:
  void InitializeCommand(const dpp::ready_t& event, const Counting& countingInstance);
  void GetUserCallback(const dpp::confirmation_callback_t& callback, const dpp::slashcommand_t& event);
  void OnCommandRun(const dpp::slashcommand_t& event);
};

#endif
