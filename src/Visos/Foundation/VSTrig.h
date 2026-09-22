#ifndef LEMBALL_VISOS_FOUNDATION_VSTRIG_H
#define LEMBALL_VISOS_FOUNDATION_VSTRIG_H

#include "CFixed.h"  // complete type
#include "CVector.h" // complete type

// SIZE 0x800
class VSTrig {
public:
	CFixed Cos(int p_angle);
	CFixed Sin(int p_angle);
	CVector Rotate(CVector p_vector, CFixed& p_sin, CFixed& p_cos);
	VSTrig();

private:
	friend class CSheepGroup;
	CFixed m_sine[512]; // 0x00
};

extern VSTrig* g_pVSTrig;
extern int g_nVSTrigSource[512];
extern unsigned int g_dwVSTrigInitialised;
#endif
