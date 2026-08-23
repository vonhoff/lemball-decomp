#ifndef LEMBALL_VISOS_ANIMATION_BASESTAT_H
#define LEMBALL_VISOS_ANIMATION_BASESTAT_H

#include "../../Common.h"
#include "../Foundation/String.h" // complete type

// SIZE 0x20
// VTABLE: LEMBALL 0x004988c8
class BaseStat {
public:
	BaseStat(char* p_arg0);
	virtual VsOStream& StreamOut(VsOStream& p_stream); // vtable+0x00
	virtual void Update(unsigned int p_value);         // vtable+0x04
	~BaseStat();
	BaseStat();

private:
	undefined4 m_unk0x04;       // 0x04
	unsigned int m_maximum;     // 0x08
	unsigned int m_minimum;     // 0x0c
	unsigned int m_total;       // 0x10
	unsigned int m_sampleCount; // 0x14
	String m_description;       // 0x18
};

#endif
