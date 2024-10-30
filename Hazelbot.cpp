#include "src/Common.h"
#include "src/Logger.h"

#include "src/Modules/Counting/Counting.h"
#include "src/Modules/Clip.h"
#include "src/Modules/ChatInteractions.h"
#include "src/Modules/Two.h"

#include "src/Commands/CStats.h"
#include "src/Commands/Quote.h"
#include "src/Commands/Wisdom.h"

int main(int argc, char* argv[]){
  // parse launch arguments (TEMPORARY SOLUTION)
  int logLevel = 2;
  if(argc > 1){
    for(int i = 1; i < argc; i++){
      if(std::strcmp(argv[i], "delete-commands") == 0){
        std::cout << "later" << std::endl;
      }
      else if(std::strcmp(argv[i], "log-level") == 0){
        logLevel = std::stoi(argv[i + 1]);
        i++;
      }
    }
  }

  // Initialize logger
  InitializeLogger(logLevel);
  Log("Logger Initialized", DEBUG, "Hazelbot");

  // Initialize core libraries
  ConfigParser::initialize_configuration();
  InitializeResponses();
  InitializeTimezoneOffset();

  // Initialize bot
  std::string token = ConfigParser::get_string("token", "");

  if(token == ""){
    // Handle error when no token is given
    Log("A token is required to start the bot. Please correctly fill in the \"token\"field in the file \"config/Hazelbot.cfg\", and restart the bot.", CRITICAL, "Hazelbot");
    exit(INVALID_TOKEN);
  }

  dpp::cluster bot(token, dpp::i_default_intents | dpp::i_message_content);

  bot.on_log(&DppLog);

  // Initialize modules
  ChatInteractions mod_chatInteractions = ChatInteractions();
  Counting mod_counting = Counting();
  Clip mod_clip = Clip();

  // Set up module callbacks
  bot.on_message_create(&TwoOnMessageSent);

  bot.on_message_create(std::bind(&ChatInteractions::OnMessageSent, &mod_chatInteractions, std::placeholders::_1));

  bot.on_message_create(std::bind(&Counting::OnMessageCreate, &mod_counting, std::placeholders::_1));

  bot.on_message_create(std::bind(&Clip::OnMessageSent, &mod_clip, std::placeholders::_1));
  bot.on_message_reaction_add(std::bind(&Clip::OnMessageReactionAdd, &mod_clip, std::placeholders::_1));

  // Initialize commands
  CStats cmd_cstats = CStats();
  Quote cmd_quote = Quote();
  Wisdom cmd_wisdom = Wisdom();

  bot.on_ready(&InitializeCommandRegisterer);

  // Set up command callbacks
  bot.on_ready(std::bind(&CStats::InitializeCommand, &cmd_cstats, std::placeholders::_1, &mod_counting));
  bot.on_slashcommand(std::bind(&CStats::OnCommandRun, &cmd_cstats, std::placeholders::_1));

  bot.on_ready(std::bind(&Quote::InitializeCommand, &cmd_quote, std::placeholders::_1));
  bot.on_message_context_menu(std::bind(&Quote::OnCommandRun, &cmd_quote, std::placeholders::_1));
  bot.on_message_reaction_add(std::bind(&Quote::OnMessageReactionAdd, &cmd_quote, std::placeholders::_1));

  bot.on_ready(std::bind(&Wisdom::InitializeCommand, &cmd_wisdom, std::placeholders::_1));
  bot.on_slashcommand(std::bind(&Wisdom::OnCommandRun, &cmd_wisdom, std::placeholders::_1));

  // Start bot
  bot.start(dpp::st_wait);
}
