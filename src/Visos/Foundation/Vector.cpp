#include "Vector.h"

// 68K 0x10119d00 __ct__7CVectorFv
// STUB: LEMBALL 0x0041a3c0
Vector::Vector()
{
}

// 68K 0x101195da __ml__FRC7CVectori
// STUB: LEMBALL 0x00422380
Vector operator*(const Vector& p_vector, int p_scale)
{
	return *(Vector*) 0;
}

// 68K 0x1011c116 __pl__FRC7CVectorRC7CVector
// STUB: LEMBALL 0x0044b660
Vector operator+(const Vector& p_left, const Vector& p_right)
{
	return *(Vector*) 0;
}

// Confirmed class-scoped globals.
// GLOBAL: LEMBALL 0x0049e0d0
Vector g_aFormationTemplates[24];
