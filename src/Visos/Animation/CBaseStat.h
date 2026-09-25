#ifndef LEMBALL_VISOS_ANIMATION_CBASESTAT_H
#define LEMBALL_VISOS_ANIMATION_CBASESTAT_H

#include "../Foundation/CString.h"
#include "Visos/Foundation/CArenaBase.h"

// SIZE 0x20
// VTABLE: LEMBALL 0x00498960
class CBaseStat : public CArenaBase {
public:
	CBaseStat(char* p_description);
	virtual CVSOStream& StreamOut(CVSOStream& p_stream); // vtable+0x00
	virtual void Update(unsigned int p_value);           // vtable+0x04
	~CBaseStat();

	friend class CGame;

private:
	unsigned int m_maximum;     // 0x08
	unsigned int m_minimum;     // 0x0c
	unsigned int m_total;       // 0x10
	unsigned int m_sampleCount; // 0x14
	CString m_description;      // 0x18
};

#endif
