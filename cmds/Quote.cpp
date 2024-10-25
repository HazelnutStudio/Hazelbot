#include "Quote.h"
#include <dpp/dispatcher.h>

std::string Quote::getReactionEmoji(bool inText = false){
  std::string emojiId = ConfigParser::get_string("quote_reaction_emoji_id", "");
  std::string emojiName = ConfigParser::get_string("quote_reaction_emoji_name", "");

  if(emojiName == ""){
    // no emoji given, get default instead
    return u8"⭐";
  }
  if(emojiId == ""){
    // no emoji id given, emoji is probably a default emoji
    return emojiName;
  }
  else{
    // emoji id was given, emoji is probably a custom emoji
    // this also needs to be formatted differently if the emoji is used in text or as a raw emoji
    // so that's annoying
    std::string emojiRaw = emojiName + ":" + emojiId;
    if(inText) return "<:" + emojiRaw + ">";
    else return emojiRaw;
  }
}

void Quote::addQuoteMessage(Quote_MessageInfo& message, dpp::cluster* bot, std::string key){
  std::string timestamp = "<t:" + std::to_string(message.GetMessageSent() + GetTimezoneOffset()) + ":f>";
  std::string userMention = "<@" + std::to_string(message.GetMessageAuthorId()) + ">";
  std::string content = "> " + message.GetMessageContent() + "\n \\- " + userMention + ", " + timestamp;

  dpp::message m;
  m.content = content;
  m.channel_id = ConfigParser::get_string("quotes_channel_id", "0");
  Quote::ActiveVotes.erase(key); // deactivate vote
  bot->message_create(m);
}

void Quote::getQuoteVoteMessageCallback(const dpp::confirmation_callback_t& callback, const dpp::message_context_menu_t& event){
  if(callback.is_error()){
    std::cout << "uh oh.." << std::endl;
    return;
  }
  dpp::message context = event.ctx_message;
  dpp::message data = callback.get<dpp::message>();
  Quote_MessageInfo info(data.guild_id, context.id, context.author.id, context.content, context.sent);

  event.from->creator->message_add_reaction(data.id, data.channel_id, getReactionEmoji());

  std::string id = data.channel_id.str() + "." + data.id.str();
  Quote::ActiveVotes[id] = info;
}

void Quote::getReactedMessageCallback(const dpp::confirmation_callback_t& callback, const dpp::message_reaction_add_t& event){
  if(callback.is_error()){
    std::cout << "uh oh\n";
    return;
  } 

  dpp::message message = callback.get<dpp::message>();
  dpp::reaction reaction;

  for(dpp::reaction& r : message.reactions){
    bool matchingNames = r.emoji_name == event.reacting_emoji.name;
    bool matchingIds = r.emoji_id == event.reacting_emoji.id;

    if(matchingNames && matchingIds){
      reaction = r;
      break;
    }
  }

  if(reaction.count_normal - 1 >= ConfigParser::get_integer("quote_votes_required", 1)){
    // message has reached the number of quote votes required 
    // subtract one to account for bot's reaction
    
    std::string key = message.channel_id.str() + "." + message.id.str();
    if(Quote::ActiveVotes.count(key) == 0){
      // message has likely already been added to the quotes channel_id
      // while we were waiting for this callback. so ignore this
      return;
    }

    Quote_MessageInfo& info = Quote::ActiveVotes[key];
    addQuoteMessage(info, event.from->creator, key);
  }
}

bool Quote::IsQuoteMessage(dpp::snowflake channelId, dpp::snowflake messageId){
  std::string key = channelId.str() + "." + messageId.str();

  if(Quote::ActiveVotes.count(key)) return true;
  else return false;
}

void Quote::InitializeCommand(const dpp::ready_t& event){
  dpp::slashcommand command;

  command.set_type(dpp::ctxm_message);
  command.set_name("Quote");
  command.set_description("Starts a quote vote with the selected message");
  command.set_application_id(event.from->creator->me.id);

  std::string guildId = ConfigParser::get_string("guild_id", "0");

  if(guildId == "0"){
    event.from->creator->global_command_create(command);
    return;
  }

  event.from->creator->guild_command_create(command, guildId);

  Quote::ActiveVotes = {}; 
}

void Quote::OnCommandRun(const dpp::message_context_menu_t& event){
  std::string response = "<@" + std::to_string(event.command.get_issuing_user().id) + "> wants to quote the message " + event.ctx_message.get_url() + "! React with " + getReactionEmoji(true) + " to vote!";
  event.reply(response);

  // callback to get information on the just sent message, used to add this to the list of active quote votes

  std::function<void(dpp::confirmation_callback_t)> callback = std::bind(&Quote::getQuoteVoteMessageCallback, this, std::placeholders::_1, event);
  event.get_original_response(dpp::command_completion_event_t(callback));
}

void Quote::OnMessageReactionAdd(const dpp::message_reaction_add_t& event){
  if(!IsQuoteMessage(event.channel_id, event.message_id)){
    std::cout << event.channel_id.str() << "." << event.message_id.str() << "\n";
    return;
  }  
  
  if(event.reacting_emoji.format() != getReactionEmoji()){
    // wrong emoji was used
    return;
  }

  dpp::command_completion_event_t callback = std::bind(&Quote::getReactedMessageCallback, this, std::placeholders::_1, event);
  event.from->creator->message_get(event.message_id, event.channel_id, callback);
}
