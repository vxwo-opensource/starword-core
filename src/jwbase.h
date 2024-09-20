#ifndef JWBASE_H_
#define JWBASE_H_

#include <cstdint>

#if defined(_MSC_VER)
#include <BaseTsd.h>
typedef SSIZE_T ssize_t;
#else
#include <sys/types.h>
#endif

typedef uint16_t JWChar;
typedef JWChar *JWCharBuffer;

#ifdef NDEBUG
#define DebugOutput(...)
#else
void DebugOutput(const char *format, ...);
#endif

#endif
