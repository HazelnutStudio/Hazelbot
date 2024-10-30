#include "EmojiHandler.h"

std::string GetEmoji(std::string name, bool inText, std::string defaultEmoji){
  std::string emojiId = ConfigParser::get_string(name + "_id", "");
  std::string emojiName = ConfigParser::get_string(name + "_name", "");

  if(emojiName == ""){
    // no emoji given, get default instead
    return defaultEmoji;
  }
  else if(emojiId == ""){
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
