#ifndef LEMBALL_VISOS_MESSAGING_CREADNCMSBUFF_H
#define LEMBALL_VISOS_MESSAGING_CREADNCMSBUFF_H

#include "CReadMsBuff.h" // complete type

// SIZE 0x28
// VTABLE: LEMBALL 0x004991d8
class CReadNcmsBuff : public CReadMsBuff {
public:
	CReadMsBuff* UpdateSubPacket();
	CReadNcmsBuff(unsigned long p_arg0, unsigned long p_arg1, int p_arg2, unsigned short p_arg3);
	~CReadNcmsBuff();

private:
	int m_messageCount;                  // 0x18
	int m_firstMessageId;                // 0x1c
	CReadMsBuff** m_messages;            // 0x20
	unsigned int m_nextExpectedSequence; // 0x24
};

// SYNTHETIC: LEMBALL 0x00462a60
// CReadNcmsBuff::`scalar deleting destructor'

#endif
