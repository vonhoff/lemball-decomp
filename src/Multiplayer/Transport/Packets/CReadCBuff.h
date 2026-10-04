#ifndef LEMBALL_VISOS_MESSAGING_CREADCBUFF_H
#define LEMBALL_VISOS_MESSAGING_CREADCBUFF_H

#include "CReadPacketBuff.h"

class CReadPacket;

// SIZE 0x14
class CReadCBuff : public CReadPacketBuff {
public:
	CReadCBuff(int p_packetCount, unsigned short p_packetSize);
	CReadPacket* NextPacketReady();
	CReadPacket* StorePacket();

private:
	int m_nextSequence;        // 0x0c
	unsigned int m_outOfOrder; // 0x10

	friend class CReadSocket;
};

#endif
