#ifndef LEMBALL_VISOS_MESSAGING_READPACKET_H
#define LEMBALL_VISOS_MESSAGING_READPACKET_H

#include "../../Common.h"
#include "../Foundation/Critical.h"
#include "BasePacket.h" // complete type

// SIZE 0x2c
// VTABLE: LEMBALL 0x004991c0 Critical
// VTABLE: LEMBALL 0x004991c8 BasePacket
class ReadPacket : public BasePacket, public Critical {
public:
	ReadPacket(unsigned short p_arg0);
	virtual ~ReadPacket();
	void Fill(const unsigned char* p_arg0, unsigned short p_arg1);

	friend class NetworkOptionsProc;
	friend class BaseFrontendProcess;
	friend class Broadcast;
	friend class ReadPacketBuff;
	friend class ReadNcBuff;
	friend class ReadCBuff;
	friend class ReadSocket;

private:
	unsigned int m_used;  // 0x24
	unsigned int m_ready; // 0x28
};

#endif
