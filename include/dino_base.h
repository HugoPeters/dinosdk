#ifndef _DINO_BASE_H_
#define _DINO_BASE_H_

#include <stdbool.h>

typedef unsigned char uint8;
typedef unsigned int uint32;
typedef unsigned long long uint64;

#define NULL 0

#ifdef __GNUC__
#define MAKE_VISIBLE __attribute__((visibility("default")))
#define PREVENT_OPTIMIZE __attribute__((used))
#define DLL_IMPORT __attribute__((weak))
#define DLL_EXPORT MAKE_VISIBLE PREVENT_OPTIMIZE
#else // mainly to make intellisense play nice
#define MAKE_VISIBLE
#define PREVENT_OPTIMIZE
#define DLL_IMPORT
#define DLL_EXPORT
#endif

#endif // _DINO_BASE_H_