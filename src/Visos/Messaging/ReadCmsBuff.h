#ifndef LEMBALL_SCAFFOLD_VISOS_MESSAGING_READCMSBUFF_H
#define LEMBALL_SCAFFOLD_VISOS_MESSAGING_READCMSBUFF_H

#include "../../Common.h"
#include "ReadMsBuff.h" // complete type

// SIZE 0x20
class ReadCmsBuff : public ReadMsBuff {
public:
	ReadCmsBuff(int p_arg0, int p_arg1, unsigned short p_arg2);
	int StoreSubPacket();
	~ReadCmsBuff();

private:
	int m_messageCount;      // 0x18
	ReadMsBuff** m_messages; // 0x1c
};

#endif
