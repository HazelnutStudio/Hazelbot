#include "BotResponses.h"

const std::string RESPONSE_FILE_PATH = "config/responses";
std::unordered_map<std::string, std::vector<std::string>> botResponses;

std::vector<std::string> getResponseFiles(){
  std::vector<std::string> files;

  for(const auto& entry : std::filesystem::directory_iterator(RESPONSE_FILE_PATH)){
    std::cout << "Found responses file \"" << entry.path() << "\n";
    files.push_back(entry.path());
  }

  return files;
}

std::vector<std::string> parseResponseFile(std::string path){
  std::ifstream file(path);
  std::stringstream ss;

  ss << file.rdbuf();

  std::vector<std::string> tokens = StringUtils::split_string_on_nl(ss.str());
  return tokens;
}

void InitializeResponses(){
  std::vector<std::string> files = getResponseFiles();

  for(std::string path : files){
    // get filename
    std::string filename = path.substr(path.find_last_of("/\\") + 1);
    // remove file extension
    std::string::size_type const p(filename.find_last_of('.'));
    std::string responseId = filename.substr(0, p);

    std::vector<std::string> responses = parseResponseFile(path);
    botResponses.insert({responseId, responses});
  }
}

std::string GetResponse(std::string responseId){
  if(botResponses.count(responseId) == 0){
    std::cerr << "No responses found for \"" << responseId << "\". Please create the file.";
    return "Error getting response - No responses for \"" + responseId + "\" were found. Please see the log for more information";
  }

  std::vector<std::string>* possibleResponses = &botResponses.at(responseId);

  if(possibleResponses->size() == 0){
    std::cerr << "Response file for \"" << responseId << "\" is empty! No responses found.";
    return "Error getting response - No responses for \"" + responseId + "\" were found. Please see the log for more information";
  }

  if(ConfigParser::get_boolean("catgirl_mode", true)){
    std::vector<std::string> nyaa = {"meow", "mroww", "nyaa~", "mrrp", "meow~", "mrrrppp :3", "nyaa :3", "meow", "meow", "mroww", "miaou", "MRRREOWW"};
    possibleResponses = &nyaa;
  }

  int r = std::rand() % possibleResponses->size();
  return possibleResponses->at(r);
}
