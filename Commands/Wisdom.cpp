#include "Wisdom.h"

void Wisdom::InitializeCommand(const dpp::ready_t& event){
  dpp::slashcommand command;

  command.set_name("wisdom");
  command.set_description("various insightful messages, or something");
  command.set_application_id(event.from->creator->me.id);

  RegisterCommand(command);
}

void Wisdom::OnCommandRun(const dpp::slashcommand_t& event){
  if(event.command.get_command_name() != "wisdom")
    return;

  event.reply(GetResponse("wisdom_cmd"));
}
