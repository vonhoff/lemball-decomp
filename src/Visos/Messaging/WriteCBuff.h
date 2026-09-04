#ifndef LEMBALL_VISOS_MESSAGING_WRITECBUFF_H
#define LEMBALL_VISOS_MESSAGING_WRITECBUFF_H

#include "../../Common.h"
#include "WritePacketBuff.h" // complete type

// SIZE 0x0c
class WriteCBuff : public WritePacketBuff {
public:
	WriteCBuff(int p_arg0, unsigned short p_arg1);
	bool IsPacketAvailable(unsigned short p_arg0);
	WritePacket* StorePacket(unsigned short p_arg0, const unsigned char* p_arg1, int p_arg2, NetworkMessage* p_arg3);
};

#endif
