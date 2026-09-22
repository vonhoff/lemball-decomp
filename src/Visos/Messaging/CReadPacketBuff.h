#ifndef LEMBALL_VISOS_MESSAGING_CREADPACKETBUFF_H
#define LEMBALL_VISOS_MESSAGING_CREADPACKETBUFF_H

#include "CBasePacketBuff.h" // complete type

// SIZE 0x0c
class CReadPacketBuff : public CBasePacketBuff {
public:
	CReadPacketBuff(int p_arg0, unsigned short p_arg1);
	void FillPacket(int p_arg0);
	void UnUseAll();
	CReadPacketBuff();
};

#endif
