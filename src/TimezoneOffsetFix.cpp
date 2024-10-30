#include "TimezoneOffsetFix.h"
#include "Logger.h"

time_t _timezoneOffset;

time_t GetTimezoneOffset(){
  return _timezoneOffset;
}

void InitializeTimezoneOffset(){
  if(!ConfigParser::get_boolean("use_timezone_fix", false)){
    // timezone fix is not enabled, set timezone offset to 0
    Log("TimezoneOffsetFix is disabled, leaving offset as zero.", DEBUG, "TimezoneOffsetFix");
    _timezoneOffset = 0;
    return;
  }

  time_t t = time(NULL);
  struct tm localtime = {0};
  localtime_r(&t, &localtime);
  _timezoneOffset = localtime.tm_gmtoff;

  Log("TimezoneOffsetFix is enabled. Setting offset to " + std::to_string(_timezoneOffset) + ".", INFO, "TimezoneOffsetFix");
}
