#include "Two.h"

void TwoOnMessageSent(const dpp::message_create_t& event){
 if(event.msg.channel_id.str() != ConfigParser::get_string("2_channel_id", "0")){
    return;
  } 

  if(event.msg.author == event.from->creator->me){
    return;
  }

  if(event.msg.content.find("2") != std::string::npos){
    return;
  } 

  if(event.msg.content.find("two") != std::string::npos){
    return;
  }

  event.from->creator->message_delete(event.msg.id, event.msg.channel_id);
}
