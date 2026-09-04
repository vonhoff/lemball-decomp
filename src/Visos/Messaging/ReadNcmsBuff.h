#ifndef LEMBALL_VISOS_MESSAGING_READNCMSBUFF_H
#define LEMBALL_VISOS_MESSAGING_READNCMSBUFF_H

#include "../../Common.h"
#include "ReadMsBuff.h" // complete type

// SIZE 0x28
// VTABLE: LEMBALL 0x004991d8
class ReadNcmsBuff : public ReadMsBuff {
public:
	ReadMsBuff* UpdateSubPacket();
	ReadNcmsBuff(unsigned long p_arg0, unsigned long p_arg1, int p_arg2, unsigned short p_arg3);
	~ReadNcmsBuff();

private:
	int m_messageCount;                  // 0x18
	int m_firstMessageId;                // 0x1c
	ReadMsBuff** m_messages;             // 0x20
	unsigned int m_nextExpectedSequence; // 0x24
};

#endif
