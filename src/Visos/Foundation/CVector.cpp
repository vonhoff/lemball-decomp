#include "CVector.h"

#include "VsDebug.h"

// FUNCTION: LEMBALL 0x0040c290
CVector& CVector::operator=(const CVector& p_other)
{
	m_xFixed = p_other.m_xFixed;
	m_yFixed = p_other.m_yFixed;
	return *this;
}

// FUNCTION: LEMBALL 0x0041a3c0
CVector::CVector() : m_xFixed(DEBUG_SENTINEL), m_yFixed(DEBUG_SENTINEL)
{
}

// FUNCTION: LEMBALL 0x00422380
CVector operator*(const CVector& p_vector, int p_scale)
{
	int y = p_vector.m_yFixed * p_scale;
	int x = p_vector.m_xFixed * p_scale;
	return CVector(x, y);
}

// FUNCTION: LEMBALL 0x0044b640
CVector* CVector::SetIntegers(int p_x, int p_y)
{
	m_xFixed = p_x << 12;
	m_yFixed = p_y << 12;
	return this;
}

// FUNCTION: LEMBALL 0x0044b660
CVector operator+(const CVector& p_left, const CVector& p_right)
{
	int y = p_right.m_yFixed + p_left.m_yFixed;
	int x = p_right.m_xFixed + p_left.m_xFixed;
	return CVector(x, y);
}

// GLOBAL: LEMBALL 0x0049e0d0
FormationVectorTemplate g_aFormationTemplates[24] = {
	{0, 0}, {-16, 0},   {-32, 0},  {-48, 0},   {-64, 0},   {-80, 0},   {-96, 0},  {-112, 0},
	{0, 0}, {-24, -16}, {-24, 16}, {-48, -32}, {-48, 32},  {-72, -48}, {-72, 48}, {-96, -64},
	{0, 0}, {-24, -24}, {-24, 0},  {-24, 24},  {-48, -48}, {-48, -24}, {-48, 0},  {-48, 24}};
