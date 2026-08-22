#ifndef LEMBALL_VISOS_FOUNDATION_VSTRIG_H
#define LEMBALL_VISOS_FOUNDATION_VSTRIG_H

#include "../../Common.h"
#include "Fixed.h"  // complete type
#include "Vector.h" // complete type

// SIZE 0x800
class VsTrig {
public:
	Fixed Cos(int p_angle);
	Fixed Sin(int p_angle);
	Vector Rotate(Vector p_vector, Fixed& p_sin, Fixed& p_cos);
	VsTrig();

private:
	Fixed m_sine[512]; // 0x00
};

// Confirmed class-scoped globals.
extern VsTrig* g_pVSTrig;
extern int g_nVSTrigSource[512];
extern unsigned int g_dwVSTrigInitialised;
#endif
