#ifndef LEMBALL_VISOS_MESSAGING_CREADNCMSBUFF_H
#define LEMBALL_VISOS_MESSAGING_CREADNCMSBUFF_H

#include "CReadMSBuff.h" // complete type

// SIZE 0x28
// VTABLE: LEMBALL 0x004991d8
class CReadNCMSBuff : public CReadMSBuff {
public:
	CReadMSBuff* UpdateSubPacket();
	CReadNCMSBuff(unsigned long p_firstMessageId,
				  unsigned long p_lastMessageId,
				  int p_messageCapacity,
				  unsigned short p_packetSize);
	~CReadNCMSBuff();

private:
	int m_messageCount;                  // 0x18
	int m_firstMessageId;                // 0x1c
	CReadMSBuff** m_messages;            // 0x20
	unsigned int m_nextExpectedSequence; // 0x24
};

// SYNTHETIC: LEMBALL 0x00462a60
// CReadNCMSBuff::`scalar deleting destructor'

#endif
