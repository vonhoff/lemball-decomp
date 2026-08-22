#include "VsTrig.h"

// 68K 0x10119dcc Rotate__6VSTrigCF7CVectorR6CFixedR6CFixed
// FUNCTION: LEMBALL 0x0041a3e0
Vector VsTrig::Rotate(Vector p_vector, Fixed& p_sin, Fixed& p_cos)
{
	int sin = p_sin.m_value;
	int cosLo = p_cos.m_value & 0xfff;
	int y = p_vector.m_yFixed;
	int negSin = -sin;
	int x = p_vector.m_xFixed;
	int yLo = y & 0xfff;
	int yHi = y >> 12;
	int xLo = x & 0xfff;
	int xHi = x >> 12;
	int cosHi = p_cos.m_value >> 12;
	int sinHi = p_sin.m_value >> 12;
	int sinLo = p_sin.m_value & 0xfff;

	int resY = ((sinLo * yLo) >> 12) + (sinHi * yLo) + ((cosLo * xLo) >> 12) + (xLo * cosHi) + (yHi * p_sin.m_value) +
			   (xHi * p_cos.m_value);

	int negSinHi = negSin >> 12;
	int negSinLo = negSin & 0xfff;

	int resX = ((cosLo * yLo) >> 12) + ((negSinLo * xLo) >> 12) + (negSinHi * xLo) + (negSin * xHi) + (yLo * cosHi) +
			   (yHi * p_cos.m_value);

	Vector res;
	res.m_xFixed = resX;
	res.m_yFixed = resY;
	return res;
}

// 68K 0x10119f6e Sin__6VSTrigCFi
// FUNCTION: LEMBALL 0x0044b6a0
Fixed VsTrig::Sin(int p_angle)
{
	if (p_angle < 0) {
		return Fixed(-m_sine[(-p_angle) % 512].m_value);
	}
	return Fixed(m_sine[p_angle % 512].m_value);
}

// 68K 0x10119f28 Cos__6VSTrigCFi
// FUNCTION: LEMBALL 0x0044b6f0
Fixed VsTrig::Cos(int p_angle)
{
	int angle = p_angle + 128;
	if (angle < 0) {
		return Fixed(-m_sine[(-128 - p_angle) % 512].m_value);
	}
	return Fixed(m_sine[angle % 512].m_value);
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
