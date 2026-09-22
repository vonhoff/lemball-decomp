#ifndef LEMBALL_VISOS_MESSAGING_CWRITEPACKETBUFF_H
#define LEMBALL_VISOS_MESSAGING_CWRITEPACKETBUFF_H

#include "CBasePacketBuff.h" // complete type

class CNetworkMessage;

// SIZE 0x0c
class CWritePacketBuff : public CBasePacketBuff {
public:
	CWritePacketBuff(int p_arg0, unsigned short p_arg1);
	void FillPacket(int p_arg0, const unsigned char* p_arg1, unsigned short p_arg2, CNetworkMessage* p_arg3);
	CWritePacketBuff();
};

#endif
