#ifndef LEMBALL_VISOS_MESSAGING_CWRITEPACKETBUFF_H
#define LEMBALL_VISOS_MESSAGING_CWRITEPACKETBUFF_H

#include "CBasePacketBuff.h"

class CNetworkMessage;

// SIZE 0x0c
class CWritePacketBuff : public CBasePacketBuff {
public:
	CWritePacketBuff(int p_packetCount, unsigned short p_packetSize);
	void FillPacket(int p_index, const unsigned char* p_data, unsigned short p_size, CNetworkMessage* p_message);
	CWritePacketBuff();
};

#endif
