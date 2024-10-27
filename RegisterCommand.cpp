#include "RegisterCommand.h"
#include "Logger.h"

dpp::cluster* _bot;
std::string _guildId;

void InitializeCommandRegisterer(const dpp::ready_t& event){
  _bot = event.from->creator;
  _guildId = ConfigParser::get_string("guild_id", "0");

  if(_guildId == "0"){
    Log("Config entry \"guild_id\" was left blank, registering commands globally. This behaviour is not yet fully supported.", WARNING, "RegisterCommand");
  }
}

void RegisterCommand(dpp::slashcommand& command){
  Log("Registering command \"" + command.name + "\".", DEBUG, "RegisterCommand");

  if(_guildId == "0"){
    // register command globally
    _bot->global_command_create(command);
  }
  else{
    // register command for the given guild
    _bot->guild_command_create(command, _guildId);
  }
}
