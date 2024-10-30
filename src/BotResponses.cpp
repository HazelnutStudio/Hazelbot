#include "BotResponses.h"
#include "Logger.h"

const std::string RESPONSE_FILE_PATH = "config/response";
std::unordered_map<std::string, std::vector<std::string>> botResponses;

std::vector<std::string> getResponseFiles(){
  std::vector<std::string> files;

  for(const auto& entry : std::filesystem::directory_iterator(RESPONSE_FILE_PATH)){
    Log("Found responses file " + entry.path().string(), DEBUG, "BotResponses");
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
  Log("Initializing responses...", DEBUG, "BotResponses");
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

  Log("Responses initialized!", INFO, "BotResponses");
}

std::string GetResponse(std::string responseId){
  if(botResponses.count(responseId) == 0){
    Log("No responses found for \"" + responseId + "\". Please create the file and restart the bot.", ERROR, "BotResponses");
    return "Error getting response. Please see the log for more information";
  }

  std::vector<std::string>* possibleResponses = &botResponses.at(responseId);

  if(possibleResponses->size() == 0){
    Log("Response file for \"" + responseId + "\" is empty. Response files require at least one response.", ERROR, "BotResponses");
    return "Error getting response. Please see the log for more information";
  }

  if(ConfigParser::get_boolean("catgirl_mode", true)){
    std::vector<std::string> nyaa = {"meow", "mroww", "nyaa~", "mrrp", "meow~", "mrrrppp :3", "nyaa :3", "meow", "meow", "mroww", "miaou", "MRRREOWW"};
    possibleResponses = &nyaa;
  }

  int r = std::rand() % possibleResponses->size();
  Log("Selected response " + std::to_string(r) + " for " + responseId + " - \"" + possibleResponses->at(r) + "\"", DEBUG, "BotResponses");
  return possibleResponses->at(r);
}
