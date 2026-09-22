#ifndef LEMBALL_VISOS_MESSAGING_CPULSEMESSAGE_H
#define LEMBALL_VISOS_MESSAGING_CPULSEMESSAGE_H

#include "CNetworkMessage.h" // complete type

// SIZE 0x2c
// VTABLE: LEMBALL 0x00499218
class CPulseMessage : public CNetworkMessage {
public:
	inline CPulseMessage() : CNetworkMessage(1) { OpenDataStream(); }
	void AddData();
	void GetData();
	~CPulseMessage() { CloseDataStream(); }
};

extern CPulseMessage* g_pPulseMessage;

// SYNTHETIC: LEMBALL 0x00462b10
// CPulseMessage::`scalar deleting destructor'

#endif
