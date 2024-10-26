#include "TimezoneOffsetFix.h"

time_t _timezoneOffset;

time_t GetTimezoneOffset(){
  return _timezoneOffset;
}

void InitializeTimezoneOffset(){
  if(!ConfigParser::get_boolean("use_timezone_fix", false)){
    // timezone fix is not enabled, set timezone offset to 0
    _timezoneOffset = 0;
  }

  time_t t = time(NULL);
  struct tm localtime = {0};
  localtime_r(&t, &localtime);
  _timezoneOffset = localtime.tm_gmtoff;
}