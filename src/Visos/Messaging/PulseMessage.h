#ifndef LEMBALL_VISOS_MESSAGING_PULSEMESSAGE_H
#define LEMBALL_VISOS_MESSAGING_PULSEMESSAGE_H

#include "../../Common.h"
#include "NetworkMessage.h" // complete type

// SIZE 0x2c
// VTABLE: LEMBALL 0x00499218
class PulseMessage : public NetworkMessage {
public:
	inline PulseMessage() : NetworkMessage(1) { OpenDataStream(); }
	void AddData();
	void GetData();
	// 68K 0x101073a2 __dt__13CPulseMessageFv
	~PulseMessage() { CloseDataStream(); }
};

extern PulseMessage* g_pPulseMessage;

// SYNTHETIC: LEMBALL 0x00462b10
// PulseMessage::`scalar deleting destructor'

#endif
