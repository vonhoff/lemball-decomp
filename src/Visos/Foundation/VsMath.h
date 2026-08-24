#ifndef LEMBALL_VISOS_FOUNDATION_VSMATH_H
#define LEMBALL_VISOS_FOUNDATION_VSMATH_H

#include "../../AI/Base/AiCoord.h" // complete type
#include "../../Common.h"
#include "../../Control/Game/Game.h" // complete type

class VsMath {
public:
	unsigned int SqRoot(unsigned int p_arg0);
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
bool CloseTo(AiCoord p_first, AiCoord p_second);
int Sgn(int p_value);

extern int g_anRotationDirections[8];
extern unsigned int g_anFacingDirectionYFlip[8];
#endif
