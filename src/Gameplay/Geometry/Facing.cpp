#include "Facing.h"

#include "AICOORD.h"
#include "Application/GameMain.h"
#include "Engine/Math/CVSMath.h"
#include "Engine/Math/FixedPoint.h"
#include "Engine/Math/FixedPointConversion.h"

#include <stdlib.h>

// GLOBAL: LEMBALL 0x0049d020
int g_anRotationDirections[FACING_DIRECTION_COUNT] = {0, 1, 1, 1, 1, -1, -1, -1};

// GLOBAL: LEMBALL 0x0049d040
unsigned int g_anFacingDirectionYFlip[FACING_DIRECTION_COUNT] = {4, 3, 2, 3, 4, 5, 6, 5};

// FUNCTION: LEMBALL 0x00413e80
unsigned int ReturnFacingDirection(int p_fromX, int p_fromY, int p_toX, int p_toY)
{
	int nDeltaX = IntegerToFixed((unsigned int) p_toX - p_fromX);
	int nDeltaY = IntegerToFixed((unsigned int) p_toY - p_fromY);

	int nAbsX = VsAbs(nDeltaX);
	int nFraction;
	int nAbsY = VsAbs(nDeltaY);

	nFraction = nAbsY & FIXED_POINT_FRACTION_MASK;
	nFraction = (nFraction * 0x6a0) >> FIXED_POINT_FRACTION_BITS;
	int nHigh = nAbsY >> FIXED_POINT_FRACTION_BITS;
	unsigned int nDirection;

	if (nHigh * 0x6a0 + nFraction > nAbsX) {
		nDirection = 0;
	}
	else if ((int) (((unsigned int) nHigh * 0x350 + nAbsY) * 2 + nFraction) > nAbsX) {
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

// FUNCTION: LEMBALL 0x00413f80
unsigned int Distance(int p_x1, int p_y1, int p_x2, int p_y2)
{
	int dx = abs((int) ((unsigned int) p_x1 - p_x2));
	int dy = abs((int) ((unsigned int) p_y1 - p_y2));
	dx = (unsigned int) dx * dx;
	dy = (unsigned int) dy * dy;
	return ((CVSMath*) g_pRandomSeed)->SqRoot((unsigned int) dy + dx);
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
		if ((int) ((unsigned int) dy * dy + (unsigned int) dx * dx) < CLOSE_TO_HORIZONTAL_DISTANCE_SQUARED_LIMIT) {
			return true;
		}
	}
	return false;
}
