#include "CountingSavesystem.h"
#include <fstream>

bool CountingSavesystem::save(CountingState state){
  nlohmann::json userdata;

  for (std::map<std::string, CountingUserState>::iterator it = state.user_stats.begin(); it != state.user_stats.end(); it++) {
    nlohmann::json data = {
          };
    userdata[it->first] = {
      {"highest_count", it->second.highest_count},
      {"highest_count_sent", it->second.highest_count_sent},
      {"total_counts", it->second.total_counts},
      {"biggest_failure", it->second.biggest_failure},
      {"total_failures", it->second.total_failures}
    };
  }

  nlohmann::json json = {
    {"current_number", state.current_number},
    {"last_count_author", state.last_count_author.str()},
    {"highest_count", state.highest_count},
    {"highest_count_sent", state.highest_count_sent},
    {"total_counts", state.total_counts},
    {"longest_chain_failed_by", state.longest_chain_failed_by.str()},
    {"total_failures", state.total_failures},
  };

  json["user_data"] = userdata;

  std::ofstream save_file;
  save_file.open("data/counting_data.json");

  if(!save_file.is_open()){
    std::cout << "error opening file" << std::endl;
  }

  save_file << json;
  save_file.close();
  return true;
}

CountingState CountingSavesystem::load(){
  std::ifstream save_file;
  save_file.open("data/counting_data.json");

  if(!save_file.is_open()){
    std::cout << "couldn't open counting save file, creating new file instead.";
    return CountingState();
  }

  std::stringstream buffer;
  buffer << save_file.rdbuf();

  nlohmann::json json;
  json = json.parse(buffer.str());

  CountingState state;

  state.current_number = json.value<int>("current_number", 0);
  state.last_count_author = json.value("last_count_author", "0");
  state.highest_count = json.value<int>("highest_count", 0);
  state.highest_count_sent = json.value<std::time_t>("highest_count_sent", std::time_t());
  state.total_counts = json.value<int>("total_counts", 0);
  state.longest_chain_failed_by = json.value("longest_chain_failed_by", "0");
  state.total_failures = json.value<int>("total_failures", 0);

  nlohmann::json userdata = json.value<nlohmann::json>("user_data", {});
  std::map<std::string, CountingUserState> users = {};

  for(nlohmann::json::iterator it = userdata.begin(); it != userdata.end(); it++){
    CountingUserState user;
    user.highest_count = it->value<int>("highest_count", 0);
    user.highest_count_sent = it->value<std::time_t>("highest_count_sent", std::time_t());
    user.total_counts = it->value<int>("total_counts", 0);
    user.biggest_failure = it->value<int>("biggest_failure", 0);
    user.total_failures = it->value<int>("total_failures", 0);
    
    std::string user_id = it.key();
    std::pair<std::string, CountingUserState> pair(user_id, user);
    users.insert(pair);
  }

  state.user_stats = users;

  return state;
}
