#include "VsTrig.h"

// 68K 0x10119dcc Rotate__6VSTrigCF7CVectorR6CFixedR6CFixed
// STUB: LEMBALL 0x0041a3e0
Vector VsTrig::Rotate(Vector p_vector, Fixed& p_sin, Fixed& p_cos)
{
	return *(Vector*) 0;
}

// 68K 0x10119f6e Sin__6VSTrigCFi
// STUB: LEMBALL 0x0044b6a0
Fixed VsTrig::Sin(int p_angle)
{
	return *(Fixed*) 0;
}

// 68K 0x10119f28 Cos__6VSTrigCFi
// STUB: LEMBALL 0x0044b6f0
Fixed VsTrig::Cos(int p_angle)
{
	return *(Fixed*) 0;
}

// 68K 0x10219096 __ct__6VSTrigFv
// STUB: LEMBALL 0x0045a940
VsTrig::VsTrig()
{
}

// Confirmed class-scoped globals.
// GLOBAL: LEMBALL 0x004a13c0
VsTrig* g_pVSTrig;

// GLOBAL: LEMBALL 0x004a13c8
int g_nVSTrigSource[512];

// GLOBAL: LEMBALL 0x004a1bc8
unsigned int g_dwVSTrigInitialised;
