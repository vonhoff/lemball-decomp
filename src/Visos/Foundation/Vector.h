#ifndef LEMBALL_VISOS_FOUNDATION_VECTOR_H
#define LEMBALL_VISOS_FOUNDATION_VECTOR_H

#include "../../Common.h"

// SIZE 0x08
class Vector {
public:
	Vector();

private:
	int m_xFixed; // 0x00
	int m_yFixed; // 0x04
};

Vector operator*(const Vector& p_vector, int p_scale);
Vector operator+(const Vector& p_left, const Vector& p_right);

// Confirmed class-scoped globals.
extern Vector g_aFormationTemplates[24];
#endif
