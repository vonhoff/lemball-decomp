#ifndef LEMBALL_VISOS_MESSAGING_CREADNCBUFF_H
#define LEMBALL_VISOS_MESSAGING_CREADNCBUFF_H

#include "CReadPacketBuff.h" // complete type

class CReadPacket;

// SIZE 0x10
class CReadNCBuff : public CReadPacketBuff {
public:
	CReadNCBuff(unsigned long p_lastMessageId, unsigned short p_packetSize);
	CReadPacket* GetPacket(unsigned long p_messageId);
	CReadPacket* UpdatePacket();

private:
	int m_messageSlots; // 0x0c
};

#endif
