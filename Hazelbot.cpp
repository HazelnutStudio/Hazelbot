#include "Common.h"
#include <cmath>
#include <fstream>
#include <sstream>
#include <variant>
#include "Counting.h"
#include "TimezoneOffsetFix.h"

#include "cmds/CStats.h"

const std::string v = "14";
std::string quotes_saveto_channel_id;
std::string counting_channel_id;
std::string channel2id;
std::string clips_channel_id;
int quote_votes_required;
int clip_votes_required;
std::map<std::string, std::vector<std::string>> bot_responses;
Counting _counting;

struct quote_message_info {
private:
	std::string guild_id;
	std::string quoted_message;
	dpp::snowflake quoted_message_author;
	std::string quoted_message_content;
	std::time_t quoted_message_sent;

public:
	quote_message_info(std::string _guild_id, std::string _quoted_message, dpp::snowflake _quoted_message_author, std::string _quoted_message_content, std::time_t _quoted_message_sent) {
		guild_id = _guild_id;
		quoted_message = _quoted_message;
		quoted_message_author = _quoted_message_author;
		quoted_message_content = _quoted_message_content;
		quoted_message_sent = _quoted_message_sent;
	}
	quote_message_info() {
		guild_id = "0";
		quoted_message = "0";
		quoted_message_author = "0";
		quoted_message_content = ":3";
		quoted_message_sent = 0;
	}

	std::string get_guild_id() {
		return guild_id;
	}

	std::string get_quoted_message_id() {
		return quoted_message;
	}

	std::string get_quoted_message_content() {
		return quoted_message_content;
	}

	std::time_t get_quoted_message_sent() {
		return quoted_message_sent;
	}

	dpp::snowflake get_quoted_message_author_id() {
		return quoted_message_author;
	}
};

std::map<std::string, quote_message_info> active_quote_votes;

struct clip_message_info{
public:
  std::string clip_msg_content;
  dpp::snowflake clip_submitted_by;
  
};

std::map<std::string, clip_message_info> active_clip_votes;

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

bool counting_filter(dpp::cluster& bot, const dpp::message_create_t& event){
  dpp::snowflake channel_id = event.msg.channel_id;

  if(channel_id.str() != counting_channel_id){
    // message was sent outside of the counting channel, so it should be ignored
    return false;
  }

  if(event.msg.author == bot.me){
    // message was sent by bot, so it should be ignored
    return false;
  }

  return true;
}

std::string get_quote_reaction_emoji(bool inText = false) {
	// discord's emoji system is kinda a mess, so we have to account for multiple cases here
	// first of all, default emojis all just use their unicode character
	// when writing them in code though (for the default star emoji)
	// on windows only (i think) you have to use the u8 string literal otherwise it just doesn't work
	// this is only necessary in code, if you specify the emoji from the config file it works fine
  // i don't use windows anymore!!!! (still keeping the string literal here though for anyone who does)

	// custom emojis are in the format name:id, so we need another entry in the config file for custom emojis
	// if an emoji is to be used in text, you are required to use angled brackets around the emoji
	// (unless it's a default emoji, then you can't put angled brackets around it)
	// if the emoji is to be used for a reaction, you also can't use angled brackets
	// so yeah.. kinda a mess but

	std::string reaction_emoji_id = ConfigParser::get_string("quote_reaction_emoji_id", "");
	std::string reaction_emoji_name = ConfigParser::get_string("quote_reaction_emoji_name", "");

	if (reaction_emoji_name == "") {
		// no emoji given, get default instead
		return u8"⭐";
	}

	if (reaction_emoji_id == "") {
		// no emoji id given, emoji is probably a default emoji
		return reaction_emoji_name;
	}


	std::string emoji_raw = reaction_emoji_name + ":" + reaction_emoji_id;
	if (inText) {
		return "<:" + emoji_raw + ">";
	}
	else {
		return emoji_raw;
	}
}

bool is_quote_message(dpp::snowflake channel_id, dpp::snowflake message_id) {
	std::string channel_msg_id = channel_id.str() + "." + message_id.str();
	if (active_quote_votes.count(channel_msg_id)) {
		// a key exists for the channel, message id pair
		// therefore the given message is a quote message
		return true;
	}
	return false;
}

//const std::string quotes_saveto_guild_id = ConfigParser::get_string("guild_id", "");

void add_quote_message(quote_message_info& message, dpp::cluster* bot, std::string key) {
	// called when a quote message has reached enough votes to be added to the quotes channel

	// idk why it makes me do this in multiple lines
  std::string timestamp;
	std::string content = "> " + message.get_quoted_message_content() + "\n \\- <@";
	content += message.get_quoted_message_author_id().str() + ">, <t:" + std::to_string(message.get_quoted_message_sent() + GetTimezoneOffset()) + ":f>";

	dpp::message m;
	m.content = content;
	m.channel_id = quotes_saveto_channel_id;
  active_quote_votes.erase(key); // deactivate quote vote
	bot->message_create(m);
}

void on_quote_message_reaction_add_message_get_callback(const dpp::confirmation_callback_t callback, const dpp::message_reaction_add_t& event) {
	if (callback.is_error()) {
		std::cout << "uh oh!!!! on_quote_message_reaction_add_message_get_callback" << std::endl;
		return;
	}

	dpp::message message = callback.get<dpp::message>();
	dpp::reaction reaction;
	// search message for correct reaction
	// also extremely yucky nest here but i'm not gonna do anything about it :3
	for (size_t i = 0; i < message.reactions.size(); i++)
	{
		dpp::reaction r = message.reactions[i];
		if (r.emoji_name == event.reacting_emoji.name) {
			if (r.emoji_id == event.reacting_emoji.id) {
				reaction = r;
				break;
			}
		}
	}

	if (reaction.count_normal - 1 >= quote_votes_required) {
		// message has reached number of votes required
		// find message in active_quote_votes
    std::string key = message.channel_id.str() + "." + message.id.str();
    if(active_quote_votes.count(key) == 0){
      // message has likely just been added to the quotes channel, while we were waiting on this callback 
      // if this is the case, we should just not continue
      return;
    }
		quote_message_info& info = active_quote_votes[key];
		add_quote_message(info, event.from->creator, key);
	}

}

void on_quote_message_reaction_add(const dpp::cluster& bot, const dpp::message_reaction_add_t& event) {
	// called when a reaction is added to a quote message

	// making sure it was the right reaction
	if (event.reacting_emoji.format() != get_quote_reaction_emoji()) {
		std::cout << "wrong emoji!" << std::endl;
		return;
	}

	// i hate that i have to do this.. this sucks
	dpp::command_completion_event_t callback = std::bind(on_quote_message_reaction_add_message_get_callback, std::placeholders::_1, event);
	event.from->creator->message_get(event.message_id, event.channel_id, callback);
}

void callback_quote_response_send_success(const dpp::confirmation_callback_t& callback, const dpp::message_context_menu_t& event) {
	if (callback.is_error()) {
		std::cout << "uh oh.." << std::endl;
		return;
	}
	dpp::message context = event.ctx_message;

	dpp::message data = callback.get<dpp::message>();
	quote_message_info info(std::to_string(data.guild_id), std::to_string(context.id), context.author.id, context.content, context.sent);

	// add reaction emoji
	// you can't call message_add_reaction from a constant reference to the cluster, so we have this weird thingy instead.
	// idk what it does, and it probably won't work, but if you're reading this that means it did
	event.from->creator->message_add_reaction(data.id, data.channel_id, get_quote_reaction_emoji());
	// nvm i found out what it does apparently that's just how you call methods from a pointer lmao
	std::string id = data.channel_id.str() + "." + data.id.str();
	active_quote_votes.insert({ id, info});
}

void appcmd_quote(dpp::cluster& bot, const dpp::message_context_menu_t& event) {
	std::string response = "<@" + std::to_string(event.command.get_issuing_user().id) + "> wants to quote a message! React with " + get_quote_reaction_emoji(true) + " to vote!";
	event.reply(response);
	// callback with information on the message just sent, used to add this to the list of active quote votes
	std::function<void(dpp::confirmation_callback_t)> callback = std::bind(callback_quote_response_send_success, std::placeholders::_1, event);
	event.get_original_response(dpp::command_completion_event_t(callback));
}

void add_top_clip(clip_message_info& message, dpp::cluster* bot, std::string key){
  std::string content = "> " + message.clip_msg_content + "\n \\- Submitted by <@" + message.clip_submitted_by.str() + ">";
  dpp::message m;
  m.content = content;
  m.channel_id = ConfigParser::get_string("top_clips_channel_id", "");
  active_clip_votes.erase(key);
  bot->message_create(m);
}

bool clips_filter(const dpp::message_create_t& event){
  std::string channel_id = event.msg.channel_id.str();
  if(channel_id != clips_channel_id){
    // message wasn't posted in the clips channel, so it should be ignored
    return false;
  }

  if(event.msg.author == event.from->creator->me){
    // checks if the message author was sent by the bot, if so, ignore
    return false;
  }

  const std::string valid_links[2] = {"twitch.tv/hazelnutstudio/clip", "clips.twitch.tv/"};
  for(std::string link : valid_links) {
    if(event.msg.content.find(link) != std::string::npos){
      // found valid link in message
      return true;
    }
  }

  // didn't find valid link in message
  return false;
}

void clips_message_on_send(const dpp::message_create_t& event){
  clip_message_info clip_info;
  clip_info.clip_submitted_by = event.msg.author.id;
  clip_info.clip_msg_content = event.msg.content;

  std::string key = event.msg.channel_id.str() + "." + event.msg.id.str();
  active_clip_votes.insert({key, clip_info});

  event.from->creator->message_add_reaction(event.msg.id, event.msg.channel_id, get_quote_reaction_emoji());
}

bool is_clips_message(dpp::snowflake channel_id, dpp::snowflake message_id){
  std::string key = channel_id.str() + "." + message_id.str();
  if(active_clip_votes.count(key)){
    // a key exists for this message, therefore it is a clip message
    return true;
  }
  return false;
}

void clips_message_on_reaction_add_message_get_callback(const dpp::confirmation_callback_t callback, const dpp::message_reaction_add_t& event){
  if(callback.is_error()){
    std::cerr << "clips message get failed";
    return;
  }

  dpp::message message = callback.get<dpp::message>();
  dpp::reaction reaction;
  for(dpp::reaction r : message.reactions){
    if(r.emoji_name == event.reacting_emoji.name && r.emoji_id == event.reacting_emoji.id){
      // same emoji
      reaction = r;
      break;
    }
  }

  if(reaction.count_normal - 1 >= clip_votes_required){
    // message has reached number of votes required
    // -1 to account for bot's reaction
    std::string key = message.channel_id.str() + "." + message.id.str();
    if(!active_clip_votes.count(key)){
      // message has likely just been added to the top clips channel while we were waiting on this callback 
      return;
    }
    clip_message_info& info = active_clip_votes[key];
    add_top_clip(info, event.from->creator, key);
  }
}

void clips_message_on_reaction_add(const dpp::message_reaction_add_t& event){
  if(event.reacting_emoji.format() != get_quote_reaction_emoji()){
    // wrong reaction emoji, ignore
    return;
  }

  // callback to get message
  dpp::command_completion_event_t callback = std::bind(clips_message_on_reaction_add_message_get_callback, std::placeholders::_1, event);
  event.from->creator->message_get(event.message_id, event.channel_id, callback);
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
	quotes_saveto_channel_id = ConfigParser::get_string("quotes_channel_id", "");
	quote_votes_required = ConfigParser::get_integer("quote_votes_required", 1);
  clip_votes_required = ConfigParser::get_integer("clip_votes_required", 1);
  counting_channel_id = ConfigParser::get_string("counting_channel_id", "");
  clips_channel_id = ConfigParser::get_string("clips_channel_id", "");

  bot_responses = parse_responses_from_files();

  InitializeTimezoneOffset();

	dpp::cluster bot(token, dpp::i_default_intents | dpp::i_message_content);

	bot.on_log(dpp::utility::cout_logger());

	bot.on_ready([&bot](const dpp::ready_t& event) {
		// initialize commands

    dpp::slashcommand quote_command;
		  quote_command.set_type(dpp::ctxm_message)
		  	.set_name("Quote")
		  	.set_description("Starts a quote vote with the selected message")
		  	.set_application_id(bot.me.id);

		if (ConfigParser::does_key_exist("guild_id")) {
			std::string guild_id = ConfigParser::get_string("guild_id", "0");
      
      if(delete_commands){
        bot.guild_bulk_command_delete(guild_id);
      }

			bot.guild_command_create(quote_command, guild_id);
		}
		else {
			std::cout << "No guild ID specified in config/hazelbot.cfg, registering all commands as public (this can take a while to sync, so it is recommended to set a guild id)" << std::endl;

      if(delete_commands){
        bot.global_bulk_command_delete();
      }

			bot.global_command_create(quote_command);
		}
	});

	bot.on_message_context_menu([&bot](const dpp::message_context_menu_t& event) {
		if (event.command.type == dpp::it_application_command) {
			//dpp::command_interaction cmd = std::get<dpp::command_interaction>(event.command.data);
			if (event.command.get_command_name() == "Quote") appcmd_quote(bot, event);
		}
	});

	// Message recieved
	bot.on_message_create([&bot](const dpp::message_create_t& event) {
		text_interactions(bot, event);
		if (two_filter(bot, event)) {
			bot.message_delete(event.msg.id, event.msg.channel_id);
		}
    if(clips_filter(event)){
      clips_message_on_send(event);
    }
	});

	bot.on_message_reaction_add([&bot](const dpp::message_reaction_add_t& event) {
		if (is_quote_message(event.channel_id, event.message_id)) {
			on_quote_message_reaction_add(bot, event);
		}
    if(is_clips_message(event.channel_id, event.message_id)){
      clips_message_on_reaction_add(event);
    }
	});

  _counting = Counting();
  bot.on_message_create(std::bind(&Counting::OnMessageCreate, _counting, std::placeholders::_1));
  
  // initialize commands
  CStats cmd_cstats = CStats();
  bot.on_ready(std::bind(&CStats::InitializeCommand, cmd_cstats, std::placeholders::_1, _counting));
  bot.on_slashcommand(std::bind(&CStats::OnCommandRun, cmd_cstats, std::placeholders::_1));

	bot.start(dpp::st_wait);
	return 0;
}
