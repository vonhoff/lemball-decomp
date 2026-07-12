#ifndef LEMBALL_FIXED_MATH_H
#define LEMBALL_FIXED_MATH_H

#include "engine/common.h"

int *LEMBALL_FASTCALL InitializeSignedTrigTable(int *pTrigTableBuffer);
int *LEMBALL_FASTCALL WriteDebugSentinelDword(int *pTarget);
unsigned int IntegerSqrtFloor(unsigned int uValue);

#endif
