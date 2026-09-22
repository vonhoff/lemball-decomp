#ifndef LEMBALL_VISOS_MESSAGING_CWRITECBUFF_H
#define LEMBALL_VISOS_MESSAGING_CWRITECBUFF_H

#include "CWritePacketBuff.h" // complete type

class CNetworkMessage;
class CWritePacket;

// SIZE 0x0c
class CWriteCBuff : public CWritePacketBuff {
public:
	CWriteCBuff(int p_arg0, unsigned short p_arg1);
	bool IsPacketAvailable(unsigned short p_arg0);
	CWritePacket* StorePacket(unsigned short p_arg0, const unsigned char* p_arg1, int p_arg2, CNetworkMessage* p_arg3);
};

#endif
