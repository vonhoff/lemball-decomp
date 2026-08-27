#ifndef LEMBALL_VISOS_MESSAGING_BROADCASTMESSAGE_H
#define LEMBALL_VISOS_MESSAGING_BROADCASTMESSAGE_H

#include "../../Common.h"
#include "NetworkMessage.h" // complete type

// SIZE 0x30
// VTABLE: LEMBALL 0x00498ea0
class BroadcastMessage : public NetworkMessage {
public:
	virtual bool GetHeader(); // vtable+0x04
	virtual void AddHeader(); // vtable+0x0c
	virtual ~BroadcastMessage();

protected:
	const char* m_header; // 0x2c
};

// SYNTHETIC: LEMBALL 0x00462810
// BroadcastMessage::`scalar deleting destructor'


#endif

