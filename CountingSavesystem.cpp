#include "CountingSavesystem.h"
#include <fstream>

bool CountingSavesystem::save(CountingState state){
  nlohmann::json json = {
    {"current_number", state.current_number},
    {"last_count_author", state.last_count_author.str()}
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

  state.current_number = json["current_number"].template get<int>();
  state.last_count_author = json["last_count_author"].template get<std::string>();

  return state;
}
