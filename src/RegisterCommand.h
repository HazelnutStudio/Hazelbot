#ifndef REGISTER_COMMAND_H
#define REGISTER_COMMAND_H

#include <dpp/dpp.h> 
#include "ConfigParser.h"

void InitializeCommandRegisterer(const dpp::ready_t& event);
void RegisterCommand(dpp::slashcommand& command);

#endif
