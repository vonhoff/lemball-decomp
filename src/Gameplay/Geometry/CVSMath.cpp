#include "CVSMath.h"

#include "Game/CGame.h"
#include "AICOORD.h"
#include "Visos/Math/FixedPoint.h"

#include <stdlib.h>

#pragma intrinsic(abs)

// FUNCTION: LEMBALL 0x00406bc0
unsigned int __stdcall CalculatePowerOfTwo(unsigned int p_exponent)
{
	unsigned int result = 1;
	while (p_exponent != 0) {
		result *= 2;
		p_exponent--;
	}
	return result;
}

// FUNCTION: LEMBALL 0x00406be0
unsigned int __stdcall ExtractBitField(unsigned int p_value, unsigned int p_shift, unsigned int p_width)
{
	register unsigned int mask = CalculatePowerOfTwo(p_width);
	mask--;
	mask &= p_value >> p_shift;
	return mask;
}

// FUNCTION: LEMBALL 0x00413e80
unsigned int ReturnFacingDirection(int p_fromX, int p_fromY, int p_toX, int p_toY)
{
	int nDeltaX = (p_toX - p_fromX) << FIXED_POINT_FRACTION_BITS;
	int nDeltaY = (p_toY - p_fromY) << FIXED_POINT_FRACTION_BITS;

	int nAbsX = VsAbs(nDeltaX);
	int nAbsY = VsAbs(nDeltaY);

	int nFraction = nAbsY & FIXED_POINT_FRACTION_MASK;
	nFraction = (nFraction * 0x6a0) >> FIXED_POINT_FRACTION_BITS;
	int nHigh = nAbsY >> FIXED_POINT_FRACTION_BITS;
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
		nDirection = (-(int) nDirection) & FACING_DIRECTION_MASK;
	}
	if (nDeltaY > 0) {
		nDirection = g_anFacingDirectionYFlip[nDirection];
	}
	return nDirection;
}

// FUNCTION: LEMBALL 0x00413f50
int WithinRect(int p_x, int p_y, int p_minX, int p_minY, int p_maxX, int p_maxY)
{
	return p_x > p_minX && p_maxX > p_x && p_minY < p_y && p_y < p_maxY;
}

// FUNCTION: LEMBALL 0x00413f80
unsigned int Distance(int p_x1, int p_y1, int p_x2, int p_y2)
{
	int dx = abs(p_x1 - p_x2);
	int dy = abs(p_y1 - p_y2);
	dx = dx * dx;
	dy = dy * dy;
	return ((CVSMath*) g_pRandomSeed)->SqRoot(dy + dx);
}

// FUNCTION: LEMBALL 0x004140d0
bool CloseTo(AICOORD p_first, AICOORD p_second)
{
	enum {
		CLOSE_TO_MAX_VERTICAL_DISTANCE = 16,
		CLOSE_TO_HORIZONTAL_DISTANCE_SQUARED_LIMIT = 100
	};
	int dx = (p_first.m_xFixed >> FIXED_POINT_FRACTION_BITS) - (p_second.m_xFixed >> FIXED_POINT_FRACTION_BITS);
	int dy = (p_first.m_yFixed >> FIXED_POINT_FRACTION_BITS) - (p_second.m_yFixed >> FIXED_POINT_FRACTION_BITS);
	int dz = (p_first.m_zFixed >> FIXED_POINT_FRACTION_BITS) - (p_second.m_zFixed >> FIXED_POINT_FRACTION_BITS);
	if (dz < 0) {
		dz = -dz;
	}
	if (dz <= CLOSE_TO_MAX_VERTICAL_DISTANCE) {
		if (dy * dy + dx * dx < CLOSE_TO_HORIZONTAL_DISTANCE_SQUARED_LIMIT) {
			return true;
		}
	}
	return false;
}

// FUNCTION: LEMBALL 0x0045a9b0
unsigned int CVSMath::SqRoot(unsigned int p_value)
{
	enum {
		SQRT_INPUT_LIMIT_2_8 = 0x100,
		SQRT_INPUT_LIMIT_2_14 = 0x4000,
		SQRT_INPUT_LIMIT_2_20 = 0x100000,
		SQRT_INPUT_LIMIT_2_24 = 0x1000000,
		SQRT_INPUT_LIMIT_2_26 = 0x4000000,
		SQRT_INPUT_LIMIT_2_28 = 0x10000000,
		SQRT_INPUT_LIMIT_2_30 = 0x40000000,
		SQRT_ROOT_LOWER_BOUND_2_4 = 0x10,
		SQRT_ROOT_LOWER_BOUND_2_7 = 0x80,
		SQRT_ROOT_LOWER_BOUND_2_10 = 0x400,
		SQRT_ROOT_LOWER_BOUND_2_12 = 0x1000,
		SQRT_ROOT_LOWER_BOUND_2_13 = 0x2000,
		SQRT_ROOT_LOWER_BOUND_2_14 = 0x4000,
		SQRT_ROOT_LOWER_BOUND_2_15 = 0x8000
	};
	unsigned int uHigh;
	unsigned int uLow;
	unsigned int uMid;

	if (p_value > SQRT_INPUT_LIMIT_2_26) {
		if (p_value > SQRT_INPUT_LIMIT_2_30) {
			uHigh = (p_value >> 15) + 1;
			uLow = SQRT_ROOT_LOWER_BOUND_2_15;
		}
		else if (p_value > SQRT_INPUT_LIMIT_2_28) {
			uHigh = (p_value >> 14) + 1;
			uLow = SQRT_ROOT_LOWER_BOUND_2_14;
		}
		else {
			uHigh = (p_value >> 13) + 1;
			uLow = SQRT_ROOT_LOWER_BOUND_2_13;
		}
	}
	else if (p_value > SQRT_INPUT_LIMIT_2_14) {
		if (p_value > SQRT_INPUT_LIMIT_2_24) {
			uHigh = (p_value >> FIXED_POINT_FRACTION_BITS) + 1;
			uLow = SQRT_ROOT_LOWER_BOUND_2_12;
		}
		else if (p_value > SQRT_INPUT_LIMIT_2_20) {
			uHigh = (p_value >> 10) + 1;
			uLow = SQRT_ROOT_LOWER_BOUND_2_10;
		}
		else {
			uHigh = (p_value >> 7) + 1;
			uLow = SQRT_ROOT_LOWER_BOUND_2_7;
		}
	}
	else if (p_value > SQRT_INPUT_LIMIT_2_8) {
		uHigh = (p_value >> 4) + 1;
		uLow = SQRT_ROOT_LOWER_BOUND_2_4;
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
int g_anRotationDirections[FACING_DIRECTION_COUNT] = {0, 1, 1, 1, 1, -1, -1, -1};

// GLOBAL: LEMBALL 0x0049d040
unsigned int g_anFacingDirectionYFlip[FACING_DIRECTION_COUNT] = {4, 3, 2, 3, 4, 5, 6, 5};
