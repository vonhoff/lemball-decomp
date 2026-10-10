#ifndef LEMBALL_VISOS_FOUNDATION_VSTRIG_H
#define LEMBALL_VISOS_FOUNDATION_VSTRIG_H

#include "CFixed.h"
#include "CVector.h"

enum {
	TRIG_ANGLE_FULL_TURN = 512,
	TRIG_ANGLE_HALF_TURN = TRIG_ANGLE_FULL_TURN / 2,
	TRIG_ANGLE_QUARTER_TURN = TRIG_ANGLE_FULL_TURN / 4,
	TRIG_ANGLE_EIGHTH_TURN = TRIG_ANGLE_FULL_TURN / 8,
	TRIG_TABLE_SIZE = TRIG_ANGLE_FULL_TURN
};

// SIZE 0x800
class VSTrig {
public:
	CFixed Cos(int p_angle) const;
	CFixed Sin(int p_angle) const;
	CVector Rotate(CVector p_vector, CFixed& p_sin, CFixed& p_cos) const;
	CVector Rotate(CVector& p_vector, int p_angle);
	VSTrig();

private:
	friend class CSheepGroup;
	friend class CFormationManager;
	CFixed m_sine[TRIG_TABLE_SIZE]; // 0x00
};

extern VSTrig* g_pVSTrig;
extern int g_nVSTrigSource[TRIG_TABLE_SIZE];
extern unsigned int g_dwVSTrigInitialised;
inline CVector VSTrig::Rotate(CVector& p_vector, int p_angle)
{
	CFixed sine = Sin(p_angle);
	CFixed cosine = Sin((unsigned int) p_angle + TRIG_ANGLE_QUARTER_TURN);
	return Rotate(p_vector, sine, cosine);
}

#endif
