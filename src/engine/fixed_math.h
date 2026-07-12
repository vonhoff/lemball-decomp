#ifndef LEMBALL_FIXED_MATH_H
#define LEMBALL_FIXED_MATH_H

#include "engine/common.h"

int *LEMBALL_FASTCALL InitializeSignedTrigTable(int *pTrigTableBuffer);
int *LEMBALL_FASTCALL WriteDebugSentinelDword(int *pTarget);
unsigned int IntegerSqrtFloor(unsigned int uValue);

struct VSMATH_Point2D {
    short m_nX;
    short m_nY;

    int Equals(const VSMATH_Point2D *pOther);
};

struct VSMATH_Fixed12Vector2D {
    int m_nX;
    int m_nY;

    VSMATH_Fixed12Vector2D *InitializeFromPixels(int nX, int nY);
    VSMATH_Fixed12Vector2D *InitializeFromPixelsThunk(int nX, int nY);
};

#endif
