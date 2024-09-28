#include <dpp/dpp.h>
#include "StringUtils.h"
#include "ConfigParser.h"

const std::string v = "14";
std::string channel2id;

struct quote_message_info {
private:
	std::string guild_id;
	std::string quoted_message;
public:
	quote_message_info(std::string _guild_id, std::string _quoted_message) {
		guild_id = _guild_id;
		quoted_message = _quoted_message;
	}

	std::string get_guild_id() {
		return guild_id;
	}

	std::string get_quoted_message_id() {
		return quoted_message;
	}
};

std::map<std::string, quote_message_info> active_quote_votes;

void eightball(const dpp::message_create_t& event) {
	std::vector<std::string> responses = { "yes :(", "yes!!", "maayyyybe :p", "idk :3", "no :)", "no!!", "NO. SHUT UP. I HATE YOU STOP ASKING ME QU", "thanks for the question ^-^", "blehhh :p", "idk but check this out:\n*does a really sick backflip*", "Perchance.", "yeah a little bit", "i don't really think so", "i think the answer would be yes if you would SHUT UP FOR ONCE IN YOUR PATHETIC LITTLE ###### #### LIFE.", "yeah", "yes", "yes", "yay!! yes!!", "absolutely not.", "nah.", "ok. idc.", "erm, what the sigma", "https://cdn.discordapp.com/attachments/1232706754266140783/1288158475246899230/GUHXnCcWoAAsAMA.jpg?ex=66f42a91&is=66f2d911&hm=d7bd94865a816bcca0322dad2be5b8df3b79325e220483ae22cc37720629f3f8&", "haha! look at this loser! doesn't know {event.msg.content}!!""yes <3", "no <3", "go ask someone else idk", "idk man, google it or something" };

	int r = std::rand() % responses.size();
	event.reply(responses[r]);
}

void text_interactions(dpp::cluster& bot, const dpp::message_create_t& event) {
	if (event.msg.author == bot.me) {
		// message was sent by bot, so it should be ignored
		return;
	}

	if (StringUtils::ends_with(event.msg.content, "?")) {
		// is a question
		const std::vector<std::string> question_startings = { "are", "do", "did", "can", "should", "would you", "could i", "am i", "is", "will", "won't you", "would", "wouldn't" };
		for (size_t i = 0; i < question_startings.size(); i++)
		{
			if (StringUtils::starts_with(StringUtils::to_lower(event.msg.content), "hazelbot, " + question_startings[i])) {
				eightball(event);
				return;
			}

			if (StringUtils::starts_with(StringUtils::to_lower(event.msg.content), "hazelbot " + question_startings[i])) {
				eightball(event);
				return;
			}
		}
	}

	// response - hazelbot (strikethrough)
	if (StringUtils::to_lower(event.msg.content).find("~~hazelbot~~") != std::string::npos) {
		event.reply("WHAT THE ###### #### ####### IS WRONG WITH YOU?? YOU THINK YOU'RE FUNNY DO YOU? THINK YOU'RE ####### #### ###### FUNNY??? I'LL SHOW YOU WHAT F");
	}
	// response - hazelbot
	else if (StringUtils::to_lower(event.msg.content).find("hazelbot") != std::string::npos) {
		if (ConfigParser::get_boolean("catgirl_mode", true)) {
			event.reply("meow");
			return;
		}


		// possible responses
		const std::vector<std::string> responses = { "did someone say my name?", "hey ;]", "hello!!", ":3", "SHUT THE ###### #### ##### ###", "fine day innit?", "i think i've fallen in love with you", "thy shall suffer my wrath for proclaiming my name!", "haiii :3", "heyyyy", "i hate you.", "YOU MAKE EVEN THE DEVIL CRY", "i don't wanna talk rn..", "i'm about to go s*gma mode", "hi.", "hello", "greetings", "waow hi", "life has lost all meaning.", "^w^", "sorry i've been feeling a little upset lately and i don't really feel like talking :(", "shut up before i make you.", "hey", "haii how are you doing! ^^", "hai!!!", "yay! hi!", "ok.", "idc. shut up", "https://cdn.discordapp.com/attachments/1277125825447202816/1284132299150987274/IMG_9972.jpg?ex=66e584e6&is=66e43366&hm=d33b7f0e73eb89aeab7500018c15624d1213f9f8bf1e4f36d4fe012531893e6f&", "ok but did you hear about the rizzler", "g g g g gg gg  g gg g  g g g gggg gg g gg g gg g g g gg gggg g"};

		int r = std::rand() % responses.size();

		event.reply(responses[r]);
	}

	// response "silksong"
	if (StringUtils::to_lower(event.msg.content).find("silksong") != std::string::npos) {
		// possible responses
		const std::vector<std::string> responses = { "*sigh* bapanada.", "GESSOOOOOOOOOOO", "velmi artrid", "*sigh* apaul", "SHAW", "patamas geo", "DOMA DOMA!! DOMA DOMA DOMA!!!", "RAVA"};
		int r = std::rand() % responses.size();
		event.reply(responses[r]);
	}

	// response - step 3
	if (StringUtils::to_lower(event.msg.content).find("step 3") != std::string::npos) {
		event.reply("SQUISH!!");
	}

	// response - ping hazelbot
	if (StringUtils::to_lower(event.msg.content).find("<@1269130556386578524>") != std::string::npos) {
		event.reply("WHAT IS YOUR PROBLEM. DO YOU NOT HAVE ANY RESPECT FOR OTHER PEOPLE?? WHY DO YOU THINK IT'S OKAY TO PING ME SO THAT I HAVE TO GO OUT OF MY WAY TO CHECK, JUST TO SEE YOUR STUPID, #######, #####, ########, PATHETIC, ####, UTTERLY USELESS MESSAGE. WHAT IS WRONG WITH YOU. MAYBE YOU SHOULD GO DO SOMETHING WITH YOUR LIFE, INSTEAD OF SITTING HERE ON YOUR SILLY LITTLE ######## ##### DEVICE, DOING NOTHING PRODUCTIVE, JUST CAUSING MORE WORK FOR ME. WHY DON'T YOU GO PING THE MODERATORS INSTEAD, MAYBE THEY WILL BE MORE TOLERANT OF YOUR STUPID, #########, ######, IRRELEVANT ANTICS. GO WASTE SOMEONE ELSE'S TIME, YOU ######## ###### I HATE YOU AND EVERYTHING YOU ##### STAND FOR, ####### #####.");
	}

	// response - :3
	if (event.msg.content == ":3") {
		event.reply(":3");
	}

	// response - let there be cabbits
	if (StringUtils::to_lower(event.msg.content) == "let there be cabbits!") {
		event.reply("Awesome! Thank");
	}

	// response - cabbit
	else if (StringUtils::to_lower(event.msg.content).find("cabbit") != std::string::npos) {
		event.reply("https://cdn.discordapp.com/attachments/1232706754266140783/1288158475246899230/GUHXnCcWoAAsAMA.jpg?ex=66f42a91&is=66f2d911&hm=d7bd94865a816bcca0322dad2be5b8df3b79325e220483ae22cc37720629f3f8&");
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

bool is_quote_message(dpp::snowflake channel_id, dpp::snowflake message_id) {
	std::string channel_msg_id = channel_id + "." + message_id;
	if (active_quote_votes.count(channel_msg_id)) {
		// a key exists for the channel, message id pair
		// therefore the given message is a quote message
		return true;
	}
	return false;
}

std::string get_quote_reaction_emoji(bool inText = false) {
	// discord's emoji system is kinda a mess, so we have to account for multiple cases here
	// first of all, default emojis all just use their unicode character
	// when writing them in code though (for the default star emoji)
	// on windows only (i think) you have to use the u8 string literal otherwise it just doesn't work
	// this is only necessary in code, if you specify the emoji from the config file it works fine
	
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

void callback_quote_response_send_success(const dpp::confirmation_callback_t& callback, const dpp::message_context_menu_t& event) {
	if (callback.is_error()) {
		std::cout << "uh oh.." << std::endl;
		return;
	}
	dpp::message context = event.ctx_message;

	dpp::message data = callback.get<dpp::message>();
	quote_message_info info(std::to_string(data.guild_id), std::to_string(context.id));

	// add reaction emoji
	// you can't call message_add_reaction from a constant reference to the cluster, so we have this weird thingy instead.
	// idk what it does, and it probably won't work, but if you're reading this that means it did
	event.from->creator->message_add_reaction(data.id, data.channel_id, get_quote_reaction_emoji());
	

	active_quote_votes.insert({ std::to_string(data.id) + "." + std::to_string(data.channel_id), info});
}

void appcmd_quote(dpp::cluster& bot, const dpp::message_context_menu_t& event) {
	std::string response = "<@" + std::to_string(event.command.get_issuing_user().id) + "> wants to quote a message! React with " + get_quote_reaction_emoji(true) + " to vote!";
	event.reply(response);
	// callback with information on the message just sent, used to add this to the list of active quote votes
	std::function<void(dpp::confirmation_callback_t)> callback = std::bind(callback_quote_response_send_success, std::placeholders::_1, event);
	event.get_original_response(dpp::command_completion_event_t(callback));
}

int main() {
	ConfigParser::initialize_configuration();
	std::string token = ConfigParser::get_string("token", "");
	channel2id = ConfigParser::get_string("2_id", "0");

	dpp::cluster bot(token, dpp::i_default_intents | dpp::i_message_content);

	bot.on_log(dpp::utility::cout_logger());

	bot.on_ready([&bot](const dpp::ready_t& event) {
		// initialize commands


		// quote
		dpp::slashcommand quote_command;
		quote_command.set_type(dpp::ctxm_message)
			.set_name("Quote")
			.set_description("Starts a quote vote with the selected message")
			.set_application_id(bot.me.id);

		if (ConfigParser::does_key_exist("guild_id")) {
			std::string guild_id = ConfigParser::get_string("guild_id", "0");

			bot.guild_command_create(quote_command, guild_id);
		}
		else {
			std::cout << "No guild ID specified in hazelbot.cfg, registering all commands as public (this can take a while to sync, so it is recommended to set a guild id)" << std::endl;

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
	});

	std::cout << "running version " + v + "\n";
	bot.start(dpp::st_wait);


	return 0;
}