#ifndef LEMBALL_VISOS_MESSAGING_CWRITECBUFF_H
#define LEMBALL_VISOS_MESSAGING_CWRITECBUFF_H

#include "CWritePacketBuff.h" // complete type

class CNetworkMessage;
class CWritePacket;

// SIZE 0x0c
class CWriteCBuff : public CWritePacketBuff {
public:
	CWriteCBuff(int p_packetCount, unsigned short p_packetSize);
	bool IsPacketAvailable(unsigned short p_sequence);
	CWritePacket* StorePacket(unsigned short p_sequence,
							  const unsigned char* p_data,
							  int p_size,
							  CNetworkMessage* p_message);
};

#endif
