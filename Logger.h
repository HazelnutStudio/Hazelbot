#ifndef LOGGER_H
#define LOGGER_H

#include <dpp/dpp.h>

enum LogType{
  TRACE,
  DEBUG,
  INFO,
  WARNING,
  ERROR,
  CRITICAL
};

class Logger{
private:
  int _logLevel;
public:
  Logger(int logLevel);
  void Log(std::string message, int level, std::string from = "");
  void DppLog(const dpp::log_t& event);
};

#endif
