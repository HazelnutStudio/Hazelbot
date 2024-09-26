#include <dpp/dpp.h>
#include "StringUtils.h"
#include "ConfigParser.h"

const std::string v = "12";
std::string channel2id;

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

int main() {
	ConfigParser::initialize_configuration();
	std::string token = ConfigParser::get_string("token", "");
	channel2id = ConfigParser::get_string("2_id", "0");

	dpp::cluster bot(token, dpp::i_default_intents | dpp::i_message_content);

	bot.on_log(dpp::utility::cout_logger());



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