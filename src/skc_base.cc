#include "skc_base.h"

#ifndef NDEBUG

#include <stdarg.h>
#include <stdio.h>

void DebugOutput(const char *format, ...) {
  FILE *fp = fopen("debug.log", "a+");
  if (fp != nullptr) {
    va_list args;
    va_start(args, format);
    vfprintf(fp, format, args);
    va_end(args);

    fclose(fp);
  }
}

#endif