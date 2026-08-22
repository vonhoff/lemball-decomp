#include "Vector.h"

// 68K 0x10119d00 __ct__7CVectorFv
// FUNCTION: LEMBALL 0x0041a3c0
Vector::Vector() : m_xFixed(DEBUG_SENTINEL), m_yFixed(DEBUG_SENTINEL)
{
}

// 68K 0x101195da __ml__FRC7CVectori
// FUNCTION: LEMBALL 0x00422380
Vector operator*(const Vector& p_vector, int p_scale)
{
	int y = p_vector.m_yFixed * p_scale;
	return Vector(p_vector.m_xFixed * p_scale, y);
}

// 68K 0x1011c116 __pl__FRC7CVectorRC7CVector
// FUNCTION: LEMBALL 0x0044b660
Vector operator+(const Vector& p_left, const Vector& p_right)
{
	int y = p_right.m_yFixed + p_left.m_yFixed;
	return Vector(p_right.m_xFixed + p_left.m_xFixed, y);
}

// Confirmed class-scoped globals.
// GLOBAL: LEMBALL 0x0049e0d0
Vector g_aFormationTemplates[24];
