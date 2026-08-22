#ifndef LEMBALL_SCAFFOLD_VISOS_MESSAGING_READCBUFF_H
#define LEMBALL_SCAFFOLD_VISOS_MESSAGING_READCBUFF_H

#include "../../Common.h"
#include "ReadPacketBuff.h" // complete type

// SIZE 0x14
class ReadCBuff : public ReadPacketBuff {
public:
	ReadCBuff(int p_arg0, unsigned short p_arg1);
	ReadPacket* NextPacketReady();
	ReadPacket* StorePacket();

private:
	unsigned int m_nextSequence; // 0x0c
	unsigned int m_outOfOrder;   // 0x10
};

#endif
