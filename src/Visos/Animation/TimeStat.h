#ifndef LEMBALL_VISOS_ANIMATION_TIMESTAT_H
#define LEMBALL_VISOS_ANIMATION_TIMESTAT_H

#include "../../Common.h"
#include "BaseStat.h" // complete type

// SIZE 0x28
// VTABLE: LEMBALL 0x00493028
class TimeStat : public BaseStat {
public:
	TimeStat(char* p_description) : BaseStat(p_description)
	{
		m_timingActive = 0;
		m_timingStart = 0;
	}
	virtual void Update(unsigned int p_value); // vtable+0x04

	friend class Game;

private:
	unsigned int m_timingStart;  // 0x20
	unsigned int m_timingActive; // 0x24
};

#endif
