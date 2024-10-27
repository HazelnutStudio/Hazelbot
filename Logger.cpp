#include "Logger.h"
#include <chrono>
#include <dpp/misc-enum.h>
#include <ctime>
#include <iomanip>

std::string getCurrentTimeFormatted(){
  using namespace std::chrono;
  auto now = system_clock::now();

  auto ms = duration_cast<milliseconds>(now.time_since_epoch()) % 1000;

  auto timer = system_clock::to_time_t(now);
  std::tm tm = *std::localtime(&timer);

  std::ostringstream oss;
  oss << std::put_time(&tm, "%H:%M:%S");
  oss << '.' << std::setfill('0') << std::setw(3) << ms.count();
  return oss.str();
}

Logger::Logger(int logLevel){
  _logLevel = logLevel;  
}

void Logger::Log(std::string message, int level, std::string from){
  if(level < _logLevel){
    // user has specified to not be interested in this log
    return;
  }
  std::string timestamp = '[' + getCurrentTimeFormatted() + ']';
  std::string levelFormat;
  std::string fromFormat;
  
  switch(level){
    case TRACE:
      levelFormat = "[TRACE]";
      break;
    case DEBUG:
      levelFormat = "[DEBUG]";
      break;
    case INFO:
      levelFormat = "[INFO]";
      break;
    case WARNING:
      levelFormat = "[WARNING]";
      break;
    case ERROR:
      levelFormat = "[ERROR]";
      break;
    case CRITICAL:
      levelFormat = "[CRITICAL]";
      break;
  }

  if(from == ""){
    fromFormat = "";
  }
  else{
    fromFormat = '[' + from + ']';
  }
  
  std::cout << timestamp << " " << fromFormat << " " << levelFormat << " " << message << "\n";
}

void Logger::DppLog(const dpp::log_t& event){
  std::string message = event.message;
  int level = (int)event.severity;
  Log(message, level, "DPP");
}
