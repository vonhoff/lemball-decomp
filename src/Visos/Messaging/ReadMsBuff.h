#ifndef LEMBALL_VISOS_MESSAGING_READMSBUFF_H
#define LEMBALL_VISOS_MESSAGING_READMSBUFF_H

#include "../../Common.h"

// SIZE 0x18
class ReadMsBuff {
public:
	ReadMsBuff(int p_arg0, int p_arg1, unsigned short p_arg2);
	void FillPacket();
	virtual ~ReadMsBuff();
	ReadMsBuff();

private:
	unsigned char* m_buffer;               // 0x04
	unsigned int m_subpacketPayloadSize;   // 0x08
	unsigned int m_expectedSubpacketCount; // 0x0c
	unsigned int m_receivedSubpacketCount; // 0x10
	unsigned int m_assembledSize;          // 0x14
};

#endif
