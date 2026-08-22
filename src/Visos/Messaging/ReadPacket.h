#ifndef LEMBALL_SCAFFOLD_VISOS_MESSAGING_READPACKET_H
#define LEMBALL_SCAFFOLD_VISOS_MESSAGING_READPACKET_H

#include "../../Common.h"
#include "BasePacket.h" // complete type

// SIZE 0x2c
class ReadPacket : public BasePacket {
public:
	ReadPacket(unsigned short p_arg0);
	void Fill(const unsigned char* p_arg0, unsigned short p_arg1);
	~ReadPacket();

private:
	byte* m_data;                      // 0x04
	void* m_lockVtable;                // 0x08
	undefined m_criticalSection[0x18]; // 0x0c
	unsigned int m_used;               // 0x24
	unsigned int m_ready;              // 0x28
};

#endif
