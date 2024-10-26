#include "ChatInteractions.h"

const std::vector<std::string> QUESTION_STARTINGS = {"are", "do", "did", "can", "should", "would", "could", "am i", "is", "will", "won't", "would", "wouldn't", "have" };

void ChatInteractions::OnMessageSent(const dpp::message_create_t& event){
  if(event.msg.author == event.from->creator->me) return;

  if(StringUtils::ends_with(event.msg.content, "?")){
    for(std::string starting : QUESTION_STARTINGS){
      if(StringUtils::starts_with(StringUtils::to_lower(event.msg.content), "hazelbot, " + starting)){
        event.reply(GetResponse("eightball"));
        return;
      }
      if(StringUtils::starts_with(StringUtils::to_lower(event.msg.content), "hazelbot " + starting)){
        event.reply(GetResponse("eightball"));
        return;
      }
    }
  }

  if(StringUtils::to_lower(event.msg.content).find("~~hazelbot~~") != std::string::npos){
    event.reply(GetResponse("strikethrough"));
  }

  else if(StringUtils::to_lower(event.msg.content).find("hazelbot") != std::string::npos){
    event.reply(GetResponse("hazelbot"));
  }

  else if(StringUtils::to_lower(event.msg.content).find("silksong") != std::string::npos){
    event.reply(GetResponse("silksong"));
  }

  else if(StringUtils::to_lower(event.msg.content).find("step 3") != std::string::npos){
    event.reply(GetResponse("SQUISH"));
  }

  else if(event.msg.content.find("<@1269130556386578524>") != std::string::npos){
    event.reply(GetResponse("ping"));
  }

  else if(event.msg.content == ":3"){
    event.reply(":3");
  }

  else if(StringUtils::to_lower(event.msg.content) == "let there be cabbits!"){
    event.reply(GetResponse("let there be cabbits"));
  }

  else if(StringUtils::to_lower(event.msg.content).find("cabbit") != std::string::npos){
    event.reply(GetResponse("cabbit"));
  }
}
