#include "CStats.h"

const Counting* _countingInstance;

void CStats::InitializeCommand(const dpp::ready_t& event, const Counting& countingInstance){
  dpp::slashcommand command;

  command.set_name("cstats");
  command.set_description("Shows stats for the counting channel");
  command.set_application_id(event.from->creator->me.id);

  dpp::command_option userOption(dpp::co_user, "user", "A specific user you would like to get stats on", false);

  command.add_option(userOption);

  // might need a better way to handle this idk
  std::string guildId = ConfigParser::get_string("guild_id", "0");

  if(guildId == "0"){
    event.from->creator->global_command_create(command);
  }

  event.from->creator->guild_command_create(command, guildId);
  _countingInstance = &countingInstance;
}

void CStats::GetUserCallback(const dpp::confirmation_callback_t& callback, const dpp::slashcommand_t& event){
  dpp::embed embed;
  dpp::message message;
  dpp::user_identified user = callback.get<dpp::user_identified>();
  
  embed.set_title("Counting Stats - " + user.global_name);


  if(_countingInstance->State.user_stats.count(user.id.str()) == 0){
    embed.set_description("User hasn't interacted with the counting system before");
    return;
  }
  
  const CountingUserState& userStats = _countingInstance->State.user_stats.at(user.id.str());


  // find contribution stats top 1 decimal place 
  std::string percentageContributionCounts = std::to_string(std::round(((double)userStats.total_counts / (double)_countingInstance->State.total_counts) * 1000) / 10);
  std::string percentageContributionFailures = std::to_string(std::round(((double)userStats.total_failures / (double)_countingInstance->State.total_failures) * 1000) / 10);

  // remove trailing zeroes
  percentageContributionCounts.erase(percentageContributionCounts.find_last_not_of('0') + 1, std::string::npos);
  percentageContributionCounts.erase(percentageContributionCounts.find_last_not_of('.') + 1, std::string::npos);

  percentageContributionFailures.erase(percentageContributionFailures.find_last_not_of('0') + 1, std::string::npos);
  percentageContributionFailures.erase(percentageContributionFailures.find_last_not_of('.') + 1, std::string::npos);

  embed.set_description("**Highest Count:** " + std::to_string(userStats.highest_count)
    + " (<t:" + std::to_string(userStats.highest_count_sent + GetTimezoneOffset()) + ":R>)"
    + "\n**Total Counts** " + std::to_string(userStats.total_counts)
    + " (" + percentageContributionCounts + "%)"
    + "\n**Biggest Failure:** " + std::to_string(userStats.biggest_failure)
    + "\n**Total Failures:** " + std::to_string(userStats.total_failures)
    + " (" + percentageContributionFailures + "%)");
  
  embed.set_thumbnail(user.get_avatar_url());
  message.add_embed(embed);
  event.reply(message);

}

void CStats::OnCommandRun(const dpp::slashcommand_t& event){
  if(event.command.get_command_name() != "cstats"){
    std::cout << "wrong command loser" << std::endl;
    return;
  }
  std::cout << "hi" << std::endl;

  dpp::embed embed;
  dpp::message message;

  dpp::command_value commandValue = event.get_parameter("user");
  if(std::holds_alternative<std::monostate>(commandValue)){
    // no parameters given in the command
    embed.set_title("Counting Stats - Global");
    embed.set_description("## Information\n**CurrentNumber:** " + std::to_string(_countingInstance->State.current_number)
      + "\n**Last Author:** <@" + std::to_string(_countingInstance->State.last_count_author) + ">"
      + "\n\n## Statistics\n**Longest Chain:** " + std::to_string(_countingInstance->State.highest_count)
      + " (<t:" + std::to_string(_countingInstance->State.highest_count_sent + GetTimezoneOffset()) + ":R>)"
      + "\n**Longest Chain Ruined By:** <@" + std::to_string(_countingInstance->State.longest_chain_failed_by) + ">"
      + "\n**Total Counts:** " + std::to_string(_countingInstance->State.total_counts)
      + "\n**Total Failures:** " + std::to_string(_countingInstance->State.total_failures));
    embed.set_thumbnail(event.command.get_guild().get_icon_url());
  }
  else{
    // a user was given in the command option
    // but we have to do a callback to get some information on the user
    dpp::snowflake userId = std::get<dpp::snowflake>(event.get_parameter("user"));
    std::function<void(dpp::confirmation_callback_t)> callback = std::bind(&CStats::GetUserCallback, this, std::placeholders::_1, event);
    event.from->creator->user_get(userId, callback);
  }

  message.add_embed(embed);
  event.reply(message);
}
