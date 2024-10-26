#include "Clip.h"

Clip_MessageInfo::Clip_MessageInfo(std::string messageContent, dpp::snowflake author){
  _messageContent = messageContent;
  _author = author;
}

std::string Clip_MessageInfo::GetMessageContent(){
  return _messageContent;
}

dpp::snowflake Clip_MessageInfo::GetMessageAuthor(){
  return _author;
}

void Clip::addTopClip(Clip_MessageInfo& message, dpp::cluster* bot, std::string key){
  std::string userMention = "<@" + message.GetMessageAuthor().str() + ">";
  std::string content = "> " + message.GetMessageContent() + "\n \\- Submitted by " + userMention;

  dpp::message m;
  m.content = content;
  m.channel_id = ConfigParser::get_string("top_clips_channel_id", "");
  
  ActiveVotes.erase(key);
  bot->message_create(m);
}

bool Clip::isClipsMessage(dpp::snowflake channelId, dpp::snowflake messageId){
  std::string key = channelId.str() + "." + messageId.str();
  if(ActiveVotes.count(key)){
    // a key exists for this message, therefore it is a clips message
    return true;
  }
  return false;
}

void Clip::onMessageReactionAddGetMessageCallback(const dpp::confirmation_callback_t& callback, const dpp::message_reaction_add_t& event){
  if(callback.is_error()){
    std::cerr << "clips message get failed\n";
    return;
  } 

  dpp::message message = callback.get<dpp::message>();
  dpp::reaction reaction;

  for(dpp::reaction r : message.reactions){
    bool matchingName = r.emoji_name == event.reacting_emoji.name;
    bool matchingId = r.emoji_id == event.reacting_emoji.id;

    if(matchingName && matchingId){
      reaction = r;
      break;
    }
  }

  if(reaction.count_normal - 1 >= ConfigParser::get_integer("clip_votes_required", 1)){
    std::string key = message.channel_id.str() + "." + message.id.str();
    if(!ActiveVotes.count(key)){
      // message has likely just been added to the top clips channel while we were waiting on this callback
      return;
    }

    Clip_MessageInfo& info = ActiveVotes.at(key);
    addTopClip(info, event.from->creator, key);
  }
}

void Clip::OnMessageSent(const dpp::message_create_t& event){
  // message wasn't sent in the clips channel
  if(event.msg.channel_id.str() != ConfigParser::get_string("clips_channel_id", "0")) return;

  // message was sent by bot, ignore
  if(event.msg.author == event.from->creator->me) return;

  // to do: make this customizable
  const std::string validLinks[2] ={
    "twitch.tv/hazelnutstudio/clip",
    "clips.twitch.tv/"
  };

  bool validClipMessage;
  for(std::string link : validLinks){
    if(event.msg.content.find(link) != std::string::npos){
      // found valid link in message
      validClipMessage = true;
    }
  }

  if(!validClipMessage) return;

  Clip_MessageInfo clipInfo(event.msg.content, event.msg.author.id);
  std::string key = event.msg.channel_id.str() + "." + event.msg.id.str();
  ActiveVotes.insert({key, clipInfo});

  // to do: make the clip vote reaction customizable
  event.from->creator->message_add_reaction(event.msg.id, event.msg.channel_id, u8"⭐");
}

void Clip::OnMessageReactionAdd(const dpp::message_reaction_add_t& event){
  if(!isClipsMessage(event.channel_id, event.message_id)){
    return;
  }

  if(event.reacting_emoji.format() != u8"⭐"){
    // wrong emoji reaction, ignore
    return;
  }

  // callback to get message
  dpp::command_completion_event_t callback = std::bind(&Clip::onMessageReactionAddGetMessageCallback, this, std::placeholders::_1, event);
  event.from->creator->message_get(event.message_id, event.channel_id, callback);
}