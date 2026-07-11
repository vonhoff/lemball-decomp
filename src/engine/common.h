#ifndef LEMBALL_ENGINE_COMMON_H
#define LEMBALL_ENGINE_COMMON_H

#include <stddef.h>

#define LEMBALL_ARRAY_COUNT(x) (sizeof(x) / sizeof((x)[0]))

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned long u32;
typedef signed char s8;
typedef signed short s16;
typedef signed long s32;

#if defined(_MSC_VER) || defined(__WATCOMC__)
#define LEMBALL_FASTCALL __fastcall
#else
#define LEMBALL_FASTCALL
#endif

#endif
