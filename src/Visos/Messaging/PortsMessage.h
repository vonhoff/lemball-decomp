#ifndef LEMBALL_VISOS_MESSAGING_PORTSMESSAGE_H
#define LEMBALL_VISOS_MESSAGING_PORTSMESSAGE_H

#include "../../Common.h"
#include "NetworkMessage.h" // complete type

// SIZE 0x30
// VTABLE: LEMBALL 0x0049a4d8
class PortsMessage : public NetworkMessage {
public:
	PortsMessage();
	bool AnyUsed();
	virtual void AddData();  // vtable+0x10
	virtual void GetData();  // vtable+0x08
	virtual ~PortsMessage(); // vtable+0x14

private:
	unsigned char* m_useCounts; // 0x2c
};

// SYNTHETIC: LEMBALL 0x0047b890
// PortsMessage::`scalar deleting destructor'

#endif
