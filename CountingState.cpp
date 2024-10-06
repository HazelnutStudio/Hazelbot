#include "CountingState.h"

CountingState::CountingState(){
  current_number = 1;
  last_count_author = 0;
  highest_count = 0;
  total_counts = 0;
  longest_chain_failed_by = 0;
  total_failures = 0;
}

}
