#ifndef LEMBALL_VISOS_FOUNDATION_VECTOR_H
#define LEMBALL_VISOS_FOUNDATION_VECTOR_H

#include "../../Common.h"
#include "Vector.h" // complete type

// SIZE 0x08
class Vector {
public:
	Vector();
	inline Vector(int p_x, int p_y) : m_xFixed(p_x), m_yFixed(p_y) {}
	inline Vector(const Vector& p_other) : m_xFixed(p_other.m_xFixed), m_yFixed(p_other.m_yFixed) {}
	Vector* SetIntegers(int p_x, int p_y);

	int m_xFixed; // 0x00
	int m_yFixed; // 0x04
};

Vector operator*(const Vector& p_vector, int p_scale);
Vector operator+(const Vector& p_left, const Vector& p_right);

extern Vector g_aFormationTemplates[24];
#endif
