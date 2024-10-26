#include "BotResponses.h"
#include "Common.h"
#include "TimezoneOffsetFix.h"

#include "Counting.h"
#include "Clip.h"
#include "ChatInteractions.h"
#include "Two.h"

#include "cmds/CStats.h"
#include "cmds/Quote.h"

int main(int argc, char *argv[]) {
	ConfigParser::initialize_configuration();
	std::string token = ConfigParser::get_string("token", "");

  InitializeTimezoneOffset();
  InitializeResponses();

	dpp::cluster bot(token, dpp::i_default_intents | dpp::i_message_content);

	bot.on_log(dpp::utility::cout_logger());

  // initialize modules
  ChatInteractions interactions = ChatInteractions();
  bot.on_message_create(std::bind(&ChatInteractions::OnMessageSent, &interactions, std::placeholders::_1));

  bot.on_message_create(&TwoOnMessageSent);

  Counting counting = Counting();
  bot.on_message_create(std::bind(&Counting::OnMessageCreate, &counting, std::placeholders::_1));

  Clip clip = Clip();
  bot.on_message_create(std::bind(&Clip::OnMessageSent, &clip, std::placeholders::_1));
  bot.on_message_reaction_add(std::bind(&Clip::OnMessageReactionAdd, &clip, std::placeholders::_1));
  
  // initialize commands
  CStats cmd_cstats = CStats();
  bot.on_ready(std::bind(&CStats::InitializeCommand, &cmd_cstats, std::placeholders::_1, counting));
  bot.on_slashcommand(std::bind(&CStats::OnCommandRun, &cmd_cstats, std::placeholders::_1));

  Quote cmd_quote = Quote();
  bot.on_ready(std::bind(&Quote::InitializeCommand, &cmd_quote, std::placeholders::_1));
  bot.on_message_context_menu(std::bind(&Quote::OnCommandRun, &cmd_quote, std::placeholders::_1));
  bot.on_message_reaction_add(std::bind(&Quote::OnMessageReactionAdd, &cmd_quote, std::placeholders::_1));

	bot.start(dpp::st_wait);
}
