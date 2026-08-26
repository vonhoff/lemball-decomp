#ifndef LEMBALL_VISOS_MESSAGING_BASEPACKET_H
#define LEMBALL_VISOS_MESSAGING_BASEPACKET_H

#include "../../Common.h"

// SIZE 0x08
// VTABLE: LEMBALL 0x004991cc
class BasePacket {
public:
	virtual ~BasePacket(); // vtable+0x00

	friend class NetworkOptionsProc;

protected:
	unsigned char* m_data; // 0x04
};

#endif
