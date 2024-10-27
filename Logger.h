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
public:
  void Log(std::string message, int level);
  void DppLog(const dpp::log_t& event);
};

#endif
