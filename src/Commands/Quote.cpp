#include "Quote.h"
#include <dpp/dispatcher.h>
#include <dpp/restresults.h>
#include "../EmojiHandler.h"

void Quote::addQuoteMessage(Quote_MessageInfo& message, dpp::cluster* bot, std::string key){
  Log("Adding quote message!", INFO, "Quote");
  std::string timestamp = "<t:" + std::to_string(message.GetMessageSent() + GetTimezoneOffset()) + ":f>";
  std::string userMention = "<@" + std::to_string(message.GetMessageAuthorId()) + ">";
  std::string content = "> " + message.GetMessageContent() + "\n \\- " + userMention + ", " + timestamp;

  dpp::message m;
  m.content = content;
  m.channel_id = ConfigParser::get_string("quotes_channel_id", "0");
  Quote::ActiveVotes.erase(key); // deactivate vote
  bot->message_create(m);
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

  RegisterCommand(command);

  Quote::ActiveVotes = {}; 
}

void Quote::OnCommandRun(const dpp::message_context_menu_t& event){
  Log("Recieved command run", DEBUG, "Quote");
  std::string response = "<@" + std::to_string(event.command.get_issuing_user().id) + "> wants to quote the message " + event.ctx_message.get_url() + "! React with " + GetEmoji("quote_reaction_emoji", true) + " to vote!";
  event.reply(response);

  // callback to get information on the just sent message, used to add this to the list of active quote votes

  event.get_original_response([&, event](const dpp::confirmation_callback_t& callback){
    if(callback.is_error()){
      Log("Failed to get quote vote message.", ERROR, "Quote");
      return;
    }
    dpp::message context = event.ctx_message;
    dpp::message data = callback.get<dpp::message>();
    Quote_MessageInfo info(data.guild_id, context.id, context.author.id, context.content, context.sent);

    event.from->creator->message_add_reaction(data.id, data.channel_id, GetEmoji("quote_reaction_emoji"));

    std::string id = data.channel_id.str() + "." + data.id.str();
    Quote::ActiveVotes[id] = info;
  });
}

void Quote::OnMessageReactionAdd(const dpp::message_reaction_add_t& event){
  if(!IsQuoteMessage(event.channel_id, event.message_id)){
    return;
  }

  if(event.reacting_emoji.format() != GetEmoji("quote_reaction_emoji")){
    // wrong emoji was used
    return;
  }

  event.from->creator->message_get(event.message_id, event.channel_id, [&, event](const dpp::confirmation_callback_t& callback){
    if(callback.is_error()){
      Log("Failed to get reacted message.", ERROR, "Quote");
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

      Quote_MessageInfo& info = Quote::ActiveVotes.at(key);
      addQuoteMessage(info, event.from->creator, key);
    }
  });
}
