#ifndef LEMBALL_ENGINE_COMMON_H
#define LEMBALL_ENGINE_COMMON_H

#include <stddef.h>

#define LEMBALL_ARRAY_COUNT(x) (sizeof(x) / sizeof((x)[0]))

#define DECOMP_JOIN2(A, B) A##B
#define DECOMP_JOIN(A, B) DECOMP_JOIN2(A, B)
#define DECOMP_STATIC_ASSERT(V)                                                                                        \
	namespace                                                                                                          \
	{                                                                                                                  \
	typedef int DECOMP_JOIN(decomp_static_assert_, __LINE__)[(V) ? 1 : -1];                                            \
	}
#define DECOMP_SIZE_ASSERT(T, S) DECOMP_STATIC_ASSERT(sizeof(T) == S)

// Unknown-width scalars retain their signedness ambiguity until codegen or
// preserved type evidence supports a more specific type.
typedef unsigned char undefined;
typedef unsigned short undefined2;
typedef unsigned int undefined4;

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned long u32;
typedef signed char s8;
typedef signed short s16;
typedef signed long s32;

#if defined(_MSC_VER) || defined(__WATCOMC__)
#define LEMBALL_FASTCALL __fastcall
#define LEMBALL_STDCALL __stdcall
#define LEMBALL_THISCALL __thiscall
#else
#define LEMBALL_FASTCALL
#define LEMBALL_STDCALL
#define LEMBALL_THISCALL
#endif

#if defined(__cplusplus)
#if defined(_MSC_VER) || defined(__WATCOMC__)
extern "C" int __cdecl _purecall(void);
#else
extern "C" int _purecall(void);
#endif
#endif

#endif
