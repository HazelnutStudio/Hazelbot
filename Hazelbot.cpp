#include "Common.h"

#include "Modules/Counting/Counting.h"
#include "Modules/Clip.h"
#include "Modules/ChatInteractions.h"
#include "Modules/Two.h"

#include "Commands/CStats.h"
#include "Commands/Quote.h"
#include "Commands/Wisdom.h"

dpp::cluster* Bot;

int main(int argc, char* argv[]){
  // Initialize core libraries
  ConfigParser::initialize_configuration();
  InitializeResponses();
  InitializeTimezoneOffset();

  // Initialize bot
  std::string token = ConfigParser::get_string("token", "");

  if(token == ""){
    // Handle error when no token is given
    std::cerr << "A token is required to start the bot. Please correctly fill in the \"token\" field in the file \"config/Hazelbot.cfg\", and restart the bot.\n";
    exit(INVALID_TOKEN);
  }

  {
    dpp::cluster bot(token, dpp::i_default_intents | dpp::i_message_content);
    Bot = &bot;
  }

  Bot->on_log(dpp::utility::cout_logger());

  // Initialize modules
  ChatInteractions mod_chatInteractions = ChatInteractions();
  Counting mod_counting = Counting();
  Clip mod_clip = Clip();

  // Set up module callbacks
  Bot->on_message_create(&TwoOnMessageSent);

  Bot->on_message_create(std::bind(&ChatInteractions::OnMessageSent, &mod_chatInteractions, std::placeholders::_1));

  Bot->on_message_create(std::bind(&Counting::OnMessageCreate, &mod_counting, std::placeholders::_1));

  Bot->on_message_create(std::bind(&Clip::OnMessageSent, &mod_clip, std::placeholders::_1));
  Bot->on_message_reaction_add(std::bind(&Clip::OnMessageReactionAdd, &mod_clip, std::placeholders::_1));

  // Initialize commands
  CStats cmd_cstats = CStats();
  Quote cmd_quote = Quote();
  Wisdom cmd_wisdom = Wisdom();

  Bot->on_ready(&InitializeCommandRegisterer);

  // Set up command callbacks
  Bot->on_ready(std::bind(&CStats::InitializeCommand, &cmd_cstats, std::placeholders::_1, mod_counting));
  Bot->on_slashcommand(std::bind(&CStats::OnCommandRun, &cmd_cstats, std::placeholders::_1));

  Bot->on_ready(std::bind(&Quote::InitializeCommand, &cmd_quote, std::placeholders::_1));
  Bot->on_message_context_menu(std::bind(&Quote::OnCommandRun, &cmd_quote, std::placeholders::_1));
  Bot->on_message_reaction_add(std::bind(&Quote::OnMessageReactionAdd, &cmd_quote, std::placeholders::_1));

  Bot->on_ready(std::bind(&Wisdom::InitializeCommand, &cmd_wisdom, std::placeholders::_1));
  Bot->on_slashcommand(std::bind(&Wisdom::OnCommandRun, &cmd_wisdom, std::placeholders::_1));

  // Start bot
  Bot->start(dpp::st_wait);
}
