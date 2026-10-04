#ifndef LEMBALL_VISOS_MESSAGING_CREADMSBUFF_H
#define LEMBALL_VISOS_MESSAGING_CREADMSBUFF_H

#include "CBasePacket.h"

// SIZE 0x18
// VTABLE: LEMBALL 0x004991d4
class CReadMSBuff : public CBasePacket {
public:
	CReadMSBuff(int p_messageSlots, int p_messageCapacity, unsigned short p_packetSize);
	void FillPacket();
	virtual ~CReadMSBuff();

	friend class CReadNCMSBuff;
	friend class CReadCMSBuff;

protected:
	unsigned int m_subpacketPayloadSize;   // 0x08
	unsigned int m_messageSlotCount;       // 0x0c
	unsigned int m_receivedSubpacketCount; // 0x10
	unsigned int m_assembledSize;          // 0x14
};

// SYNTHETIC: LEMBALL 0x00462a40
// CReadMSBuff::`scalar deleting destructor'

#endif
