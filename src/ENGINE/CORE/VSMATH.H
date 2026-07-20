#ifndef LEMBALL_FIXED_MATH_H
#define LEMBALL_FIXED_MATH_H

#include "ENGINE/CORE/COMMON.H"

int* LEMBALL_FASTCALL InitializeSignedTrigTable(int* pTrigTableBuffer);
int* LEMBALL_FASTCALL WriteDebugSentinelDword(int* pTarget);
unsigned int __stdcall IntegerSqrtFloor(unsigned int uValue);

struct VsMathPoint2D {
	short m_nX;
	short m_nY;

	int Equals(const VsMathPoint2D* pOther);
};

struct VsMathFixed12Vector2D {
	int m_nX;
	int m_nY;

	VsMathFixed12Vector2D* InitializeFromPixels(int nX, int nY);
	VsMathFixed12Vector2D* InitializeFromPixelsThunk(int nX, int nY);
};

#endif
