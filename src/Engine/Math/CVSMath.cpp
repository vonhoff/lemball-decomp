#include "CVSMath.h"

#include "FixedPoint.h"

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

// FUNCTION: LEMBALL 0x00413f50
int WithinRect(int p_x, int p_y, int p_minX, int p_minY, int p_maxX, int p_maxY)
{
	return p_x > p_minX && p_maxX > p_x && p_minY < p_y && p_y < p_maxY;
}

// FUNCTION: LEMBALL 0x0044c1e0
int sgn(int p_value)
{
	int res = p_value;
	if (res == 0) {
		return res;
	}
	if (res < 0) {
		return -1;
	}
	return 1;
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
	unsigned int uLow;
	unsigned int uHigh;
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
