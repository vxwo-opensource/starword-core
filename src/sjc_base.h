#ifndef SJC_BASE_H_
#define SJC_BASE_H_

#include <uchar.h>

#if defined(_MSC_VER)
#include <BaseTsd.h>
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
