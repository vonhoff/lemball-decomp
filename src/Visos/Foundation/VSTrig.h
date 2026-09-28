#ifndef LEMBALL_VISOS_FOUNDATION_VSTRIG_H
#define LEMBALL_VISOS_FOUNDATION_VSTRIG_H

#include "CFixed.h"
#include "CVector.h"

// SIZE 0x800
class VSTrig {
public:
	CFixed Cos(int p_angle);
	CFixed Sin(int p_angle);
	CVector Rotate(CVector p_vector, CFixed& p_sin, CFixed& p_cos);
	CVector Rotate(CVector& p_vector, int p_angle);
	VSTrig();

private:
	friend class CSheepGroup;
	friend class CFormationManager;
	CFixed m_sine[512]; // 0x00
};

extern VSTrig* g_pVSTrig;
extern int g_nVSTrigSource[512];
extern unsigned int g_dwVSTrigInitialised;
inline CVector VSTrig::Rotate(CVector& p_vector, int p_angle)
{
	CFixed sine = Sin(p_angle);
	CFixed cosine = Sin(p_angle + 0x80);
	return Rotate(p_vector, sine, cosine);
}

#endif
