#include "BotResponses.h"
#include "Common.h"
#include <cmath>
#include "TimezoneOffsetFix.h"

#include "Counting.h"
#include "Clip.h"
#include "ChatInteractions.h"

#include "cmds/CStats.h"
#include "cmds/Quote.h"

std::string channel2id;

bool two_filter(dpp::cluster& bot, const dpp::message_create_t& event) {
	std::string channel_id = std::to_string(event.msg.channel_id);
	if (channel_id != channel2id) {
		// message was sent outside of #2, so it should be ignored
		return false;
	}
	
	if (event.msg.author == bot.me) {
		// message was sent by bot, so it should be ignored
		return false;
	}

	if (event.msg.content.find("2") != std::string::npos) {
		return false;
	}

	if (event.msg.content.find("two") != std::string::npos) {
		return false;
	}

	return true;
}

bool delete_commands = false;
int main(int argc, char *argv[]) {

  if(argc > 1){
    if(std::strcmp(argv[1], "-delete_commands") == 0){
      delete_commands = true;
      std::cout << "deleting commands" << std::endl;
    }
  }
	ConfigParser::initialize_configuration();
	std::string token = ConfigParser::get_string("token", "");
	channel2id = ConfigParser::get_string("2_id", "0");

  InitializeTimezoneOffset();
  InitializeResponses();

	dpp::cluster bot(token, dpp::i_default_intents | dpp::i_message_content);

	bot.on_log(dpp::utility::cout_logger());

	// Message recieved
	bot.on_message_create([&bot](const dpp::message_create_t& event) {
		if (two_filter(bot, event)) {
			bot.message_delete(event.msg.id, event.msg.channel_id);
		}
	});

  // initialize modules
  ChatInteractions interactions = ChatInteractions();
  bot.on_message_create(std::bind(&ChatInteractions::OnMessageSent, &interactions, std::placeholders::_1));

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
	return 0;
}
