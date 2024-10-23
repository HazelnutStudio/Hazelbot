#pragma once
#include "Common.h"
#include <cinttypes>

class TimezoneOffsetFix{
private:
  static time_t _timezoneOffset;
public:
  static void InitializeTimezoneOffset();
  static time_t GetTimezoneOffset();
};
