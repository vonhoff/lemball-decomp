#ifndef LEMBALL_VISOS_MESSAGING_BASEPACKETBUFF_H
#define LEMBALL_VISOS_MESSAGING_BASEPACKETBUFF_H

#include "../../Common.h"

// SIZE 0x0c
class BasePacketBuff {
public:
	BasePacketBuff(int p_arg0, unsigned short p_arg1);
	~BasePacketBuff();
	BasePacketBuff();

private:
	int m_packetCount;           // 0x00
	unsigned short m_packetSize; // 0x04
	BasePacket** m_packets;      // 0x08
};

#endif
