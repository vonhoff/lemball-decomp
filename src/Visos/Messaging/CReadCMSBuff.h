#ifndef LEMBALL_VISOS_MESSAGING_CREADCMSBUFF_H
#define LEMBALL_VISOS_MESSAGING_CREADCMSBUFF_H

#include "CReadMSBuff.h"

// SIZE 0x20
// VTABLE: LEMBALL 0x004991dc
class CReadCMSBuff : public CReadMSBuff {
public:
	CReadCMSBuff(int p_messageCount, int p_messageCapacity, unsigned short p_packetSize);
	CReadMSBuff* StoreSubPacket();
	~CReadCMSBuff();

private:
	int m_messageCount;       // 0x18
	CReadMSBuff** m_messages; // 0x1c
};

// SYNTHETIC: LEMBALL 0x00462a80
// CReadCMSBuff::`scalar deleting destructor'

#endif
