#ifndef LEMBALL_VISOS_MESSAGING_CREADPACKETBUFF_H
#define LEMBALL_VISOS_MESSAGING_CREADPACKETBUFF_H

#include "CBasePacketBuff.h" // complete type

// SIZE 0x0c
class CReadPacketBuff : public CBasePacketBuff {
public:
	CReadPacketBuff(int p_packetCount, unsigned short p_packetSize);
	void FillPacket(int p_index);
	void UnUseAll();
	CReadPacketBuff();
};

#endif
