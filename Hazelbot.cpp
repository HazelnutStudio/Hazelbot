#include "Common.h"
#include <cmath>
#include <fstream>
#include <sstream>
#include "TimezoneOffsetFix.h"

#include "Counting.h"
#include "Clip.h"

#include "cmds/CStats.h"
#include "cmds/Quote.h"

std::string channel2id;
std::string clips_channel_id;
std::map<std::string, std::vector<std::string>> bot_responses;

std::vector<std::string> parse_responses_from_file(std::string path){
  std::ifstream file(path);
  std::stringstream buffer;

  buffer << file.rdbuf();

  std::vector<std::string> tokens = StringUtils::split_string_on_nl(buffer.str());

  return tokens;
}

std::vector<std::string> get_response_files(){
  std::string path = "config/response";
  std::vector<std::string> files;
  for(const auto & entry : std::filesystem::directory_iterator(path)){
    std::cout << "found file " << entry.path() << std::endl;
    files.push_back(entry.path());
  }

  return files;
}

std::map<std::string, std::vector<std::string>> parse_responses_from_files(){
  std::vector<std::string> files = get_response_files();
  std::map<std::string, std::vector<std::string>> result;
  for (int i=0; i < files.size(); i++) {
    std::string path = files[i];
    
    // gets the name of the file without the extension
    std::string filename = path.substr(path.find_last_of("/\\") + 1);
    std::string::size_type const p(filename.find_last_of('.'));
    std::string response_id = filename.substr(0, p);

    std::vector responses = parse_responses_from_file(path);
    result.insert({response_id, responses});
  }
  return result;
}


std::string get_response(std::string response_id){
  // picks a random response for response_id from the corresponding file
  std::vector<std::string> possible_responses = bot_responses[response_id];
  
  if(possible_responses.size() == 0){
    // if there are no responses in the file
    return "{empty response}";
  }

  int r = std::rand() % possible_responses.size();

  return possible_responses[r];
}

void text_interactions(dpp::cluster& bot, const dpp::message_create_t& event) {
	if (event.msg.author == bot.me) {
		// message was sent by bot, so it should be ignored
		return;
	}

	if (StringUtils::ends_with(event.msg.content, "?")) {
		// is a question
		const std::vector<std::string> question_startings = { "are", "do", "did", "can", "should", "would you", "could i", "am i", "is", "will", "won't you", "would", "wouldn't", "have" };
		for (size_t i = 0; i < question_startings.size(); i++)
		{
			if (StringUtils::starts_with(StringUtils::to_lower(event.msg.content), "hazelbot, " + question_startings[i])) {
				event.reply(get_response("eightball"));
        return;
			}

			if (StringUtils::starts_with(StringUtils::to_lower(event.msg.content), "hazelbot " + question_startings[i])) {
				event.reply(get_response("eightball"));
				return;
			}
		}
	}

	// response - hazelbot (strikethrough)
	if (StringUtils::to_lower(event.msg.content).find("~~hazelbot~~") != std::string::npos) {
		event.reply(get_response("strikethrough"));
	}
	// response - hazelbot
	else if (StringUtils::to_lower(event.msg.content).find("hazelbot") != std::string::npos) {
		if (ConfigParser::get_boolean("catgirl_mode", true)) {
			event.reply("meow");
			return;
		}
    
    event.reply(get_response("hazelbot"));
	}

	// response "silksong"
	if (StringUtils::to_lower(event.msg.content).find("silksong") != std::string::npos) {
    event.reply(get_response("silksong"));
	}

	// response - step 3
	if (StringUtils::to_lower(event.msg.content).find("step 3") != std::string::npos) {
		event.reply(get_response("SQUISH"));
	}

	// response - ping hazelbot
	if (StringUtils::to_lower(event.msg.content).find("<@1269130556386578524>") != std::string::npos) {
		event.reply(get_response("ping"));
	}

	// response - :3
	if (event.msg.content == ":3") {
		event.reply(":3");
	}

	// response - let there be cabbits
	if (StringUtils::to_lower(event.msg.content) == "let there be cabbits!") {
		event.reply(get_response("let there be cabbits"));
	}

	// response - cabbit
	else if (StringUtils::to_lower(event.msg.content).find("cabbit") != std::string::npos) {
		event.reply(get_response("cabbit"));
	}

	// debug
	if (StringUtils::to_lower(event.msg.content).find("secret dialogue") != std::string::npos && ConfigParser::does_key_exist("secret_dialogue")) {
		event.reply(ConfigParser::get_string("secret_dialogue", ":3"));
	}
};

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

  bot_responses = parse_responses_from_files();

  InitializeTimezoneOffset();

	dpp::cluster bot(token, dpp::i_default_intents | dpp::i_message_content);

	bot.on_log(dpp::utility::cout_logger());

	// Message recieved
	bot.on_message_create([&bot](const dpp::message_create_t& event) {
		text_interactions(bot, event);
		if (two_filter(bot, event)) {
			bot.message_delete(event.msg.id, event.msg.channel_id);
		}
	});

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
