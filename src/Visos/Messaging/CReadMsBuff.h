#ifndef LEMBALL_VISOS_MESSAGING_CREADMSBUFF_H
#define LEMBALL_VISOS_MESSAGING_CREADMSBUFF_H

#include "CBasePacket.h"

// SIZE 0x18
// VTABLE: LEMBALL 0x004991d4
class CReadMsBuff : public CBasePacket {
public:
	CReadMsBuff(int p_arg0, int p_arg1, unsigned short p_arg2);
	void FillPacket();
	virtual ~CReadMsBuff();

	friend class CReadNcmsBuff;
	friend class CReadCmsBuff;

protected:
	unsigned int m_subpacketPayloadSize;   // 0x08
	unsigned int m_expectedSubpacketCount; // 0x0c
	unsigned int m_receivedSubpacketCount; // 0x10
	unsigned int m_assembledSize;          // 0x14
};

// SYNTHETIC: LEMBALL 0x00462a40
// CReadMsBuff::`scalar deleting destructor'

#endif
