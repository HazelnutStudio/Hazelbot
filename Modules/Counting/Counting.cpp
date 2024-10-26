#include "Counting.h"

void Counting::saveState(){
  CountingSavesystem::save(State);
}

bool Counting::isCountingMessage(const dpp::message_create_t& event){
  dpp::snowflake channelId = event.msg.channel_id;

  if(channelId != Counting::_countingChannelID){
    // message was sent outside of counting channel, so we want to ignore it
    return false;
  }

  if(event.msg.author == event.from->creator->me){
    // this is our own message, so we want to ignore it
    return false;
  }
  
  return true;
}

int Counting::getFirstNumberInString(const std::string& str){
  // matches any number, unless it is contained in angled brackets <>
  // this is so that it can ignore emojis, pings, channel links, etc.
  // doesn't ignore links, but that should be added. i just Suck at regex

  std::smatch match;
  std::regex regex("[0-9]+");
  std::regex_search(str, match, regex);

  if(match.size() == 0){
    // no numbers in the given string, return with value -1
    return -1;
  }

  // returns the first number found
  return std::stoi(match[0].str());
}

void Counting::onFailChain(const dpp::message_create_t& event, const CountingFailChainCondition& condition){
  if(State.current_number > State.highest_count){
    // new longest chain 
    State.highest_count = State.current_number - 1;
    State.highest_count_sent = event.msg.sent + GetTimezoneOffset();
    State.longest_chain_failed_by = event.msg.author.id;
  }

  CountingUserState user;
  if(State.user_stats.count(event.msg.author.id.str())){
    // if user is already in user_stats
    user = State.user_stats[event.msg.author.id.str()];
    // otherwise leave all user stats as the default, by just not doing anything
  }

  user.total_failures++;

  if(State.current_number > user.biggest_failure){
    // this is a new biggest failure for the user, so update the records
    user.biggest_failure = State.current_number - 1;
  }

  State.user_stats.insert_or_assign(event.msg.author.id.str(), user);

  State.current_number = 1;
  State.last_count_author = 0;

  State.total_failures++;

  event.from->creator->message_add_reaction(event.msg.id, event.msg.channel_id, u8"❌");

  if(condition == WRONG_NUMBER){
    event.reply(GetResponse("counting_chain_fail_wrongnumber"));
  }
  else if(condition == DOUBLE_SEND){
    event.reply(GetResponse("counting_chain_fail_doublesend"));
  }

  saveState();
}

void Counting::onContinueChain(const dpp::message_create_t& event){
  Counting::State.last_count_author = event.msg.author.id;
  Counting::State.total_counts++;

  CountingUserState user;
  if(Counting::State.user_stats.count(event.msg.author.id.str())){
    // if the user is already in user_stats
    user = Counting::State.user_stats[event.msg.author.id.str()]; 
    // otherwise leave all the user stats as the default, by just not doing anything
  }

  user.total_counts++;
  if(Counting::State.current_number > user.highest_count){
    user.highest_count = Counting::State.current_number - 1;
    user.highest_count_sent = event.msg.sent + GetTimezoneOffset();
  }

  State.user_stats.insert_or_assign(event.msg.author.id.str(), user);
  State.current_number++;

  event.from->creator->message_add_reaction(event.msg.id, event.msg.channel_id, u8"✅");

  Counting::saveState();
}

Counting::Counting(){
  Counting::_countingChannelID = dpp::snowflake(ConfigParser::get_string("counting_channel_id", "0"));
  State = CountingSavesystem::load();
}

void Counting::OnMessageCreate(const dpp::message_create_t& event){
  if(!isCountingMessage(event))
    return;

  int sentNumber = getFirstNumberInString(event.msg.content);

  if(sentNumber == -1){
    // no number was found in the string, so -1 was returned
    return;
  }

  if(sentNumber != Counting::State.current_number){
    onFailChain(event, WRONG_NUMBER);
    return;
  }

  if(event.msg.author.id == Counting::State.last_count_author){
    onFailChain(event, DOUBLE_SEND);
    return;
  }

  onContinueChain(event);
}
