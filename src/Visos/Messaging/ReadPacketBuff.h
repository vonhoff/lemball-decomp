#ifndef LEMBALL_SCAFFOLD_VISOS_MESSAGING_READPACKETBUFF_H
#define LEMBALL_SCAFFOLD_VISOS_MESSAGING_READPACKETBUFF_H

#include "../../Common.h"
#include "BasePacketBuff.h" // complete type

// SIZE 0x0c
class ReadPacketBuff : public BasePacketBuff {
public:
	ReadPacketBuff(int p_arg0, unsigned short p_arg1);
	void FillPacket(int p_arg0);
	void UnUseAll();
	ReadPacketBuff();
};

#endif
