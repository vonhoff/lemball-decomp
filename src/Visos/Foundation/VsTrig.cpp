#include "VsTrig.h"

#include <stdlib.h>

#pragma intrinsic(abs)

// 68K 0x10119dcc Rotate__6VSTrigCF7CVectorR6CFixedR6CFixed
// STUB: LEMBALL 0x0041a3e0
Vector VsTrig::Rotate(Vector p_vector, Fixed& p_sin, Fixed& p_cos)
{
	return *(Vector*) 0;
}

// 68K 0x10119f6e Sin__6VSTrigCFi
// FUNCTION: LEMBALL 0x0044b6a0
Fixed VsTrig::Sin(int p_angle)
{
	if (p_angle < 0) {
		int angle = -p_angle;
		return Fixed(-m_sine[abs(angle) & 0x1ff].m_value);
	}
	return Fixed(m_sine[abs(p_angle) & 0x1ff].m_value);
}

// 68K 0x10119f28 Cos__6VSTrigCFi
// FUNCTION: LEMBALL 0x0044b6f0
Fixed VsTrig::Cos(int p_angle)
{
	return Sin(p_angle + 128);
}

// 68K 0x10219096 __ct__6VSTrigFv
// FUNCTION: LEMBALL 0x0045a940
VsTrig::VsTrig()
{
	if (g_dwVSTrigInitialised == 0) {
		const int* pSource = g_nVSTrigSource;
		Fixed* pTable = m_sine;
		do {
			int val = *pSource++;
			int fixedVal = val << 12;
			pTable->m_value = fixedVal / 4096;
			pTable++;
		} while (pSource < (const int*) &g_dwVSTrigInitialised);
	}
	g_dwVSTrigInitialised = 1;
}

// Confirmed class-scoped globals.
// GLOBAL: LEMBALL 0x004a13c0
VsTrig* g_pVSTrig;

// GLOBAL: LEMBALL 0x004a13c8
int g_nVSTrigSource[512];

// GLOBAL: LEMBALL 0x004a1bc8
unsigned int g_dwVSTrigInitialised;
