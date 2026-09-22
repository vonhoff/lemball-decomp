#ifndef LEMBALL_VISOS_MESSAGING_CREADCMSBUFF_H
#define LEMBALL_VISOS_MESSAGING_CREADCMSBUFF_H

#include "CReadMsBuff.h" // complete type

// SIZE 0x20
// VTABLE: LEMBALL 0x004991dc
class CReadCmsBuff : public CReadMsBuff {
public:
	CReadCmsBuff(int p_messageCount, int p_messageCapacity, unsigned short p_packetSize);
	CReadMsBuff* StoreSubPacket();
	~CReadCmsBuff();

private:
	int m_messageCount;       // 0x18
	CReadMsBuff** m_messages; // 0x1c
};

// SYNTHETIC: LEMBALL 0x00462a80
// CReadCmsBuff::`scalar deleting destructor'

#endif
