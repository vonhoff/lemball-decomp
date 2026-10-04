#ifndef LEMBALL_VISOS_FOUNDATION_CVSMATH_H
#define LEMBALL_VISOS_FOUNDATION_CVSMATH_H

#include "../../AI/Base/AICOORD.h"

enum {
	FACING_DIRECTION_COUNT = 8,
	FACING_DIRECTION_MASK = FACING_DIRECTION_COUNT - 1,
	FACING_DIRECTION_OPPOSITE_OFFSET = FACING_DIRECTION_COUNT / 2
};

class CVSMath {
public:
	unsigned int SqRoot(unsigned int p_value);
};

inline int VsAbs(int p_val)
{
	int t[2];
	int* p;
	if (p_val >= 0) {
		t[0] = p_val;
		p = &t[0];
	}
	else {
		t[1] = -p_val;
		p = &t[1];
	}
	return *p;
}

unsigned int ReturnFacingDirection(int p_fromX, int p_fromY, int p_toX, int p_toY);
unsigned int Distance(int p_x1, int p_y1, int p_x2, int p_y2);
bool CloseTo(AICOORD p_first, AICOORD p_second);
int sgn(int p_value);

extern int g_anRotationDirections[FACING_DIRECTION_COUNT];
extern unsigned int g_anFacingDirectionYFlip[FACING_DIRECTION_COUNT];
#endif
