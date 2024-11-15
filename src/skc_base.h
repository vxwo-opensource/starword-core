#ifndef SKC_BASE_H_
#define SKC_BASE_H_

#include <stddef.h>
#include <stdint.h>
#ifdef SKC_USE_UTF32
typedef uint32_t skchar_t;
#else
#define SKC_USE_UTF16
typedef uint16_t skchar_t;
#endif

#endif
