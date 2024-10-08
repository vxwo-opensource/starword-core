#include "my_assert.h"

#include <stdio.h>

namespace my {

int failed_count = 0;

int assert_failed() { return failed_count; }

void assert_output(bool passed, const char* expr, const char* func,
                   const char* file, int line) {
  if (passed) {
    fprintf(stdout, "[PASSED] %s, function %s, file %s, line %d.\n", expr, func,
            file, line);
    fflush(stdout);
  } else {
    ++failed_count;
    fprintf(stderr, "[FAILED] %s, function %s, file %s, line %d.\n", expr, func,
            file, line);
    fflush(stderr);
  }
}

}  // namespace my
