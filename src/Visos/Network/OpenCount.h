#ifndef LEMBALL_VISOS_NETWORK_OPENCOUNT_H
#define LEMBALL_VISOS_NETWORK_OPENCOUNT_H

#include "../../Common.h"
#include "../Messaging/NetworkMessage.h" // complete type

// SIZE 0x30
// VTABLE: LEMBALL 0x0049a848
class OpenCount : public NetworkMessage {
public:
	virtual void AddData(); // vtable+0x10
	virtual void GetData(); // vtable+0x08
	virtual ~OpenCount();   // vtable+0x14

private:
	unsigned short m_openCount; // 0x2c
	undefined2 m_reserved2e;    // 0x2e
};

#endif
