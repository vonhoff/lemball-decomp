#ifndef LEMBALL_VISOS_MESSAGING_CREADNCBUFF_H
#define LEMBALL_VISOS_MESSAGING_CREADNCBUFF_H

#include "CReadPacketBuff.h" // complete type

class CReadPacket;

// SIZE 0x10
class CReadNcBuff : public CReadPacketBuff {
public:
	CReadNcBuff(unsigned long p_arg0, unsigned short p_arg1);
	CReadPacket* GetPacket(unsigned long p_arg0);
	CReadPacket* UpdatePacket();

private:
	int m_messageSlots; // 0x0c
};

#endif
