#include "CStats.h"

const Counting* _countingInstance;

std::string getPercentage(double frac1, double frac2, int dp){
  double percentageRaw = frac1 / frac2;
  int factor = std::pow(10, dp + 2);
  int roundPercentage = std::round(percentageRaw * factor);
  double percentage = roundPercentage / std::pow(10, dp);

  std::string output = std::to_string(percentage);

  // remove trailing zeroes
  output.erase(output.find_last_not_of('0') + 1, std::string::npos);
  output.erase(output.find_last_not_of('.') + 1, std::string::npos);
   
  return output;
}

void CStats::InitializeCommand(const dpp::ready_t& event, Counting* countingInstance){
  dpp::slashcommand command;

  command.set_name("cstats");
  command.set_description("Shows stats for the counting channel");
  command.set_application_id(event.from->creator->me.id);

  dpp::command_option userOption(dpp::co_user, "user", "A specific user you would like to get stats on", false);

  command.add_option(userOption);

  RegisterCommand(command);

  _countingInstance = countingInstance;
}

void CStats::OnCommandRun(const dpp::slashcommand_t& event){
  if(event.command.get_command_name() != "cstats"){
    return;
  }

  Log("Recieved command run.", DEBUG, "CStats");

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

    event.from->creator->user_get(userId, [&event, &embed](const dpp::confirmation_callback_t& callback){
      dpp::message message;
      dpp::user_identified user = callback.get<dpp::user_identified>();
       
      embed.set_title("Counting Stats - " + user.global_name);


      if(_countingInstance->State.user_stats.count(user.id.str()) == 0){
        embed.set_description("User hasn't interacted with the counting system before");
        return;
      }
      
      const CountingUserState& userStats = _countingInstance->State.user_stats.at(user.id.str());

      std::string percentageContributionCounts = getPercentage(userStats.total_counts, _countingInstance->State.total_counts, 2);
      std::string percentageContributionFailures = getPercentage(userStats.total_failures, _countingInstance->State.total_failures, 2);
      
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
    });
  }

  message.add_embed(embed);
  event.reply(message);
}
