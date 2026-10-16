#ifndef LEMBALL_VISOS_ANIMATION_CTIMESTAT_H
#define LEMBALL_VISOS_ANIMATION_CTIMESTAT_H

#include "CBaseStat.h"

// SIZE 0x28
// VTABLE: LEMBALL 0x00493028
class CTimeStat : public CBaseStat {
public:
	CTimeStat(char* p_description) : CBaseStat(p_description)
	{
		m_timingActive = false;
		m_timingStart = 0;
	}
	virtual void Update(unsigned int p_value); // vtable+0x04

	friend class CGame;
	friend class CGDIDevice;

private:
	unsigned long m_timingStart; // 0x20
	bool m_timingActive;         // 0x24
};

#endif
