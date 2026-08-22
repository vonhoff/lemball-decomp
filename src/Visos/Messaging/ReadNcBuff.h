#ifndef LEMBALL_SCAFFOLD_VISOS_MESSAGING_READNCBUFF_H
#define LEMBALL_SCAFFOLD_VISOS_MESSAGING_READNCBUFF_H

#include "../../Common.h"
#include "ReadPacketBuff.h" // complete type

// SIZE 0x10
class ReadNcBuff : public ReadPacketBuff {
public:
	ReadNcBuff(unsigned long p_arg0, unsigned short p_arg1);
	ReadPacket* GetPacket(unsigned long p_arg0);
	ReadPacket* UpdatePacket();

private:
	int m_messageSlots; // 0x0c
};

#endif
