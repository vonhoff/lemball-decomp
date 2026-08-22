#include "Vector.h"

#include "Fixed.h"

// 68K 0x10119d00 __ct__7CVectorFv
// FUNCTION: LEMBALL 0x0041a3c0
Vector::Vector() : m_xFixed(DEBUG_SENTINEL), m_yFixed(DEBUG_SENTINEL)
{
}

// 68K 0x101195da __ml__FRC7CVectori
// FUNCTION: LEMBALL 0x00422380
Vector operator*(const Vector& p_vector, int p_scale)
{
	return Vector(p_vector.m_xFixed * p_scale, p_vector.m_yFixed * p_scale);
}

// 68K 0x1011c116 __pl__FRC7CVectorRC7CVector
// FUNCTION: LEMBALL 0x0044b660
Vector operator+(const Vector& p_left, const Vector& p_right)
{
	return Vector(p_left.m_xFixed + p_right.m_xFixed, p_left.m_yFixed + p_right.m_yFixed);
}

// GLOBAL: LEMBALL 0x0049e0d0
Vector g_aFormationTemplates[24] = {
	Vector(0, 0),    Vector(-16, 0),   Vector(-32, 0),   Vector(-48, 0),   Vector(-64, 0),  Vector(-80, 0),
	Vector(-96, 0),  Vector(-112, 0),  Vector(0, 0),     Vector(-24, -16), Vector(-24, 16), Vector(-48, -32),
	Vector(-48, 32), Vector(-72, -48), Vector(-72, 48),  Vector(-96, -64), Vector(0, 0),    Vector(-24, -24),
	Vector(-24, 0),  Vector(-24, 24),  Vector(-48, -48), Vector(-48, -24), Vector(-48, 0),  Vector(-48, 24)};
