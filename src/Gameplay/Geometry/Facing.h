#ifndef LEMBALL_GAMEPLAY_GEOMETRY_FACING_H
#define LEMBALL_GAMEPLAY_GEOMETRY_FACING_H

#include "AICOORD.h"
#include "Engine/Math/CVSMath.h"

enum {
	FACING_DIRECTION_COUNT = 8,
	FACING_DIRECTION_MASK = FACING_DIRECTION_COUNT - 1,
	FACING_DIRECTION_OPPOSITE_OFFSET = FACING_DIRECTION_COUNT / 2
};

unsigned int ReturnFacingDirection(int p_fromX, int p_fromY, int p_toX, int p_toY);
unsigned int Distance(int p_x1, int p_y1, int p_x2, int p_y2);
bool CloseTo(AICOORD p_first, AICOORD p_second);
int sgn(int p_value);

extern int g_anRotationDirections[FACING_DIRECTION_COUNT];
extern unsigned int g_anFacingDirectionYFlip[FACING_DIRECTION_COUNT];

#endif
