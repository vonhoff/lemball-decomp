#ifndef LEMBALL_SCAFFOLD_VISOS_MESSAGING_PULSEMESSAGE_H
#define LEMBALL_SCAFFOLD_VISOS_MESSAGING_PULSEMESSAGE_H

#include "../../Common.h"
#include "NetworkMessage.h" // complete type

// SIZE 0x2c
// VTABLE: LEMBALL 0x00499218
class PulseMessage : public NetworkMessage {
public:
	void AddData();
	void GetData();
	~PulseMessage();
};

// Confirmed class-scoped globals.
extern PulseMessage* g_pPulseMessage;
#endif
