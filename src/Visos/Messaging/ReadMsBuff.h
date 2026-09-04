#ifndef LEMBALL_VISOS_MESSAGING_READMSBUFF_H
#define LEMBALL_VISOS_MESSAGING_READMSBUFF_H

#include "../../Common.h"
#include "BasePacket.h"

// SIZE 0x18
// VTABLE: LEMBALL 0x004991d4
class ReadMsBuff : public BasePacket {
public:
	ReadMsBuff(int p_arg0, int p_arg1, unsigned short p_arg2);
	void FillPacket();
	virtual ~ReadMsBuff();
	ReadMsBuff();

	friend class ReadNcmsBuff;
	friend class ReadCmsBuff;

protected:
	unsigned int m_subpacketPayloadSize;   // 0x08
	unsigned int m_expectedSubpacketCount; // 0x0c
	unsigned int m_receivedSubpacketCount; // 0x10
	unsigned int m_assembledSize;          // 0x14
};

// SYNTHETIC: LEMBALL 0x00462a40
// ReadMsBuff::`scalar deleting destructor'

#endif
