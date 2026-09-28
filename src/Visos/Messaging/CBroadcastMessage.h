#ifndef LEMBALL_VISOS_MESSAGING_CBROADCASTMESSAGE_H
#define LEMBALL_VISOS_MESSAGING_CBROADCASTMESSAGE_H

#include "CNetworkMessage.h"

#include <string.h>

#pragma intrinsic(strlen)

// SIZE 0x30
// VTABLE: LEMBALL 0x00498ea0
class CBroadcastMessage : public CNetworkMessage {
public:
	inline CBroadcastMessage() {}
	inline CBroadcastMessage(const char* p_header)
	{
		m_header = p_header;
		m_payloadCapacity += strlen(p_header) + 1;
	}
	virtual bool GetHeader(); // vtable+0x04
	virtual void AddHeader(); // vtable+0x0c

protected:
	const char* m_header; // 0x2c
};

// SYNTHETIC: LEMBALL 0x00462810
// CBroadcastMessage::`scalar deleting destructor'

#endif
