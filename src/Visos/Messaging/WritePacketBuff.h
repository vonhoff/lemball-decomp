#ifndef LEMBALL_VISOS_MESSAGING_WRITEPACKETBUFF_H
#define LEMBALL_VISOS_MESSAGING_WRITEPACKETBUFF_H

#include "../../Common.h"
#include "BasePacketBuff.h" // complete type

// SIZE 0x0c
class WritePacketBuff : public BasePacketBuff {
public:
	WritePacketBuff(int p_arg0, unsigned short p_arg1);
	void FillPacket(int p_arg0, const unsigned char* p_arg1, unsigned short p_arg2, NetworkMessage* p_arg3);
	WritePacketBuff();
};

#endif
