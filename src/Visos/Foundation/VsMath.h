#ifndef LEMBALL_VISOS_FOUNDATION_VSMATH_H
#define LEMBALL_VISOS_FOUNDATION_VSMATH_H

#include "../../AI/Base/AiCoord.h" // complete type
#include "../../Common.h"
#include "../../Control/Game/Game.h" // complete type

class VsMath {
public:
	unsigned int SqRoot(unsigned int p_arg0);
};

unsigned int ReturnFacingDirection(int p_fromX, int p_fromY, int p_toX, int p_toY);
unsigned int Distance(int p_x1, int p_y1, int p_x2, int p_y2);
bool CloseTo(AiCoord p_first, AiCoord p_second);
int Direction(int p_arg0, int p_arg1, int p_arg2, int p_arg3);
int Sgn(int p_value);
#endif
