#include "CountingSavesystem.h"
#include <fstream>

bool CountingSavesystem::save(CountingState state){
  nlohmann::json json = {
    {"current_number", state.current_number},
    {"last_count_author", state.last_count_author.str()},
    {"highest_count", state.highest_count},
    {"total_counts", state.total_counts},
    {"longest_chain_failed_by", state.longest_chain_failed_by.str()},
    {"total_failures", state.total_failures}
  };

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
  state.total_counts = json.value<int>("total_counts", 0);
  state.longest_chain_failed_by = json.value("longest_chain_failed_by", "0");
  state.total_failures = json.value<int>("total_failures", 0);

  return state;
}
