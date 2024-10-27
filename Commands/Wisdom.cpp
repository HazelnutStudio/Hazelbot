#include "Wisdom.h"

void Wisdom::InitializeCommand(const dpp::ready_t& event){
  dpp::slashcommand command;

  command.set_name("wisdom");
  command.set_description("various insightful messages, or something");
  command.set_application_id(event.from->creator->me.id);

  std::string guildId = ConfigParser::get_string("guild_id", "0");

  if(guildId == "0"){
    // register command globally
    event.from->creator->global_command_create(command);
  }
  else{
    // register command per guild
    event.from->creator->guild_command_create(command, guildId);
  }
}

void Wisdom::OnCommandRun(const dpp::slashcommand_t& event){
  if(event.command.get_command_name() != "wisdom")
    return;

  event.reply(GetResponse("wisdom_cmd"));
}
