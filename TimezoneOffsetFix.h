#include <cinttypes>
#include <time.h>
#include "ConfigParser.h"

#ifndef TIMEZONE_OFFSET_FIX_H
#define TIMEZONE_OFFSET_FIX_H

time_t GetTimezoneOffset();
void InitializeTimezoneOffset(); 

#endif
