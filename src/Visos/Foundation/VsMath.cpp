#include "VsMath.h"

#include "Fixed.h"

#include <stdlib.h>

#pragma intrinsic(abs)

// 68K 0x107007a0 ReturnFacingDirection__Fiiii
// FUNCTION: LEMBALL 0x00413e80
unsigned int ReturnFacingDirection(int p_fromX, int p_fromY, int p_toX, int p_toY)
{
	int nDeltaX = (p_toX - p_fromX) << 12;
	int nDeltaY = (p_toY - p_fromY) << 12;

	int nAbsX = VsAbs(nDeltaX);
	int nAbsY = VsAbs(nDeltaY);

	int nFraction = nAbsY & 0xfff;
	nFraction = (nFraction * 0x6a0) >> 12;
	int nHigh = nAbsY >> 12;
	unsigned int nDirection;

	if (nHigh * 0x6a0 + nFraction > nAbsX) {
		nDirection = 0;
	}
	else if ((nHigh * 0x350 + nAbsY) * 2 + nFraction > nAbsX) {
		nDirection = 1;
	}
	else {
		nDirection = 2;
	}

	if (nDeltaX < 0) {
		nDirection = (-(int) nDirection) & 7;
	}
	if (nDeltaY > 0) {
		nDirection = g_anFacingDirectionYFlip[nDirection];
	}
	return nDirection;
}

// 68K 0x107008e6 Distance__Fiiii
// FUNCTION: LEMBALL 0x00413f80
unsigned int Distance(int p_x1, int p_y1, int p_x2, int p_y2)
{
	int dx = abs(p_x1 - p_x2);
	int dy = abs(p_y1 - p_y2);
	dx = dx * dx;
	dy = dy * dy;
	return ((VsMath*) g_pSentinel)->SqRoot(dy + dx);
}

// 68K 0x1060e9e6 CloseTo__F7AICOORD7AICOORD
// FUNCTION: LEMBALL 0x004140d0
bool CloseTo(AiCoord p_first, AiCoord p_second)
{
	int dx = (p_first.m_xFixed >> 12) - (p_second.m_xFixed >> 12);
	int dy = (p_first.m_yFixed >> 12) - (p_second.m_yFixed >> 12);
	int dz = (p_first.m_zFixed >> 12) - (p_second.m_zFixed >> 12);
	if (dz < 0) {
		dz = -dz;
	}
	if (dz <= 16) {
		if (dy * dy + dx * dx < 100) {
			return true;
		}
	}
	return false;
}

// 68K 0x102146c8 SqRoot__7CVSMathFUi
// FUNCTION: LEMBALL 0x0045a9b0
unsigned int VsMath::SqRoot(unsigned int p_value)
{
	unsigned int uHigh;
	unsigned int uLow;
	unsigned int uMid;

	if (p_value > 0x4000000) {
		if (p_value > 0x40000000) {
			uHigh = (p_value >> 15) + 1;
			uLow = 0x8000;
		}
		else if (p_value > 0x10000000) {
			uHigh = (p_value >> 14) + 1;
			uLow = 0x4000;
		}
		else {
			uHigh = (p_value >> 13) + 1;
			uLow = 0x2000;
		}
	}
	else if (p_value > 0x4000) {
		if (p_value > 0x1000000) {
			uHigh = (p_value >> 12) + 1;
			uLow = 0x1000;
		}
		else if (p_value > 0x100000) {
			uHigh = (p_value >> 10) + 1;
			uLow = 0x400;
		}
		else {
			uHigh = (p_value >> 7) + 1;
			uLow = 0x80;
		}
	}
	else if (p_value > 0x100) {
		uHigh = (p_value >> 4) + 1;
		uLow = 0x10;
	}
	else {
		uHigh = p_value + 1;
		uLow = 0;
	}

	if (uHigh == uLow) {
		return uLow;
	}

	while (uLow - uHigh != (unsigned int) -1) {
		uMid = (uLow + uHigh) >> 1;
		if (uMid * uMid <= p_value) {
			uLow = uMid;
		}
		else {
			uHigh = uMid;
		}
	}
	return uLow;
}

// GLOBAL: LEMBALL 0x0049d020
int g_anRotationDirections[8] = {0, 1, 1, 1, 1, -1, -1, -1};

// GLOBAL: LEMBALL 0x0049d040
unsigned int g_anFacingDirectionYFlip[8] = {4, 3, 2, 3, 4, 5, 6, 5};
