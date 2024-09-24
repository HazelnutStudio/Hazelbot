#include <dpp/dpp.h>

const std::string v = "7";

std::string to_lower(std::string str) {
	std::transform(str.begin(), str.end(), str.begin(), [](unsigned char c) {return std::tolower(c);});
	return str;
}

void text_interactions(dpp::cluster& bot, const dpp::message_create_t& event) {
	if (event.msg.author == bot.me) {
		// message was sent by bot, so it should be ignored
		return;
	}


	// response - hazelbot (strikethrough)
	if (to_lower(event.msg.content).find("~~hazelbot~~") != std::string::npos) {
		event.reply("WHAT THE ###### #### ####### IS WRONG WITH YOU?? YOU THINK YOU'RE FUNNY DO YOU? THINK YOU'RE ####### #### ###### FUNNY??? I'LL SHOW YOU WHAT F");
	}
	// response - hazelbot
	else if (to_lower(event.msg.content).find("hazelbot") != std::string::npos) {

		// possible responses
		const std::vector<std::string> responses = { "did someone say my name?", "hey ;]", "hello!!", ":3", "SHUT THE ###### #### ##### ###", "fine day innit?", "i think i've fallen in love with you", "thy shall suffer my wrath for proclaiming my name!", "haiii :3", "heyyyy", "i hate you.", "YOU MAKE EVEN THE DEVIL CRY", "i don't wanna talk rn..", "i'm about to go s*gma mode", "hi.", "hello", "greetings", "waow hi", "life has lost all meaning.", "^w^", "sorry i've been feeling a little upset lately and i don't really feel like talking :(", "shut up before i make you.", "hey", "haii how are you doing! ^^", "hai!!!", "yay! hi!", "ok.", "idc. shut up", "https://cdn.discordapp.com/attachments/1277125825447202816/1284132299150987274/IMG_9972.jpg?ex=66e584e6&is=66e43366&hm=d33b7f0e73eb89aeab7500018c15624d1213f9f8bf1e4f36d4fe012531893e6f&", "ok but did you hear about the rizzler" };

		int r = std::rand() % responses.size();

		event.reply(responses[r]);
	}

	// response "silksong"
	if (to_lower(event.msg.content).find("silksong") != std::string::npos) {
		// possible responses
		const std::vector<std::string> responses = { "*sigh* bapanada.", "GESSOOOOOOOOOOO", "velmi artrid", "*sigh* apaul", "SHAW", "patamas geo", "DOMA DOMA!! DOMA DOMA DOMA!!!", "RAVA" };
		int r = std::rand() % responses.size();
		event.reply(responses[r]);
	}

	// response - step 3
	if (to_lower(event.msg.content).find("step 3") != std::string::npos) {
		event.reply("SQUISH!!");
	}

	// response - ping hazelbot
	if (to_lower(event.msg.content).find("<@1269130556386578524>") != std::string::npos) {
		event.reply("WHAT IS YOUR PROBLEM. DO YOU NOT HAVE ANY RESPECT FOR OTHER PEOPLE?? WHY DO YOU THINK IT'S OKAY TO PING ME SO THAT I HAVE TO GO OUT OF MY WAY TO CHECK, JUST TO SEE YOUR STUPID, #######, #####, ########, PATHETIC, ####, UTTERLY USELESS MESSAGE. WHAT IS WRONG WITH YOU. MAYBE YOU SHOULD GO DO SOMETHING WITH YOUR LIFE, INSTEAD OF SITTING HERE ON YOUR SILLY LITTLE ######## ##### DEVICE, DOING NOTHING PRODUCTIVE, JUST CAUSING MORE WORK FOR ME. WHY DON'T YOU GO PING THE MODERATORS INSTEAD, MAYBE THEY WILL BE MORE TOLERANT OF YOUR STUPID, #########, ######, IRRELEVANT ANTICS. GO WASTE SOMEONE ELSE'S TIME, YOU ######## ###### I HATE YOU AND EVERYTHING YOU ##### STAND FOR, ####### #####.");
	}

	if (event.msg.content == ":3") {
		event.reply(":3");
	}
};

int main(int argc, char* argv[]) {
	if (argc != 2) {
		std::cout << "please include the bot token as a launch argument :3";
		return 0;
	}

	dpp::cluster bot(argv[1], dpp::i_default_intents | dpp::i_message_content);

	bot.on_log(dpp::utility::cout_logger());



	// Message recieved
	bot.on_message_create([&bot](const dpp::message_create_t& event) {
		text_interactions(bot, event);
		});

	std::cout << "running version " + v + "\n";
	bot.start(dpp::st_wait);

	return 0;
}

