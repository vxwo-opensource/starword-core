#ifndef SKC_BASE_H_
#define SKC_BASE_H_

#include <uchar.h>
#ifdef SKC_USE_U32
typedef char32_t skchar_t;
#else
#define SKC_USE_U16
typedef char16_t skchar_t;
#endif

#if defined(_MSC_VER)
#include <basetsd.h>
typedef SSIZE_T ssize_t;
#else
#include <sys/types.h>
#endif

#endif
