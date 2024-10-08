#include <dpp/dpp.h>
#include "CountingState.h"

#ifndef COUNTING_SAVESYSTEM_H
#define COUNTING_SAVESYSTEM_H

class CountingSavesystem{
public:
  static bool save(CountingState status);
  static CountingState load();
};

#endif
