#ifndef SJC_BASE_H_
#define SJC_BASE_H_

#include <stdint.h>
#ifndef SJCHAR_IS_U32
  typedef uint16_t sjchar_t;
#else
  typedef unit32_t sjchar_t;
#endif

#if defined(_MSC_VER)
#include <basetsd.h>
typedef SSIZE_T ssize_t;
#else
#include <sys/types.h>
#endif

#ifdef NDEBUG
#define DebugOutput(...)
#else
  void DebugOutput(const char *format, ...);
#endif

#endif
