#ifndef LEMBALL_VISOS_MESSAGING_ACKMESSAGE_H
#define LEMBALL_VISOS_MESSAGING_ACKMESSAGE_H

#include "../../Common.h"
#include "NetworkMessage.h" // complete type

// SIZE 0x30
// VTABLE: LEMBALL 0x00499230
class AckMessage : public NetworkMessage {
public:
	void AddData();
	void GetData();
	~AckMessage();

private:
	unsigned short m_packetSequence;    // 0x2c
	unsigned short m_subpacketSequence; // 0x2e
};

extern AckMessage* g_pAckMessage;

// SYNTHETIC: LEMBALL 0x00462b80
// AckMessage::`scalar deleting destructor'

#endif

