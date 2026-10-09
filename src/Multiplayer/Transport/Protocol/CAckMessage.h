#ifndef LEMBALL_VISOS_MESSAGING_CACKMESSAGE_H
#define LEMBALL_VISOS_MESSAGING_CACKMESSAGE_H

#include "CNetworkMessage.h"

// SIZE 0x30
// VTABLE: LEMBALL 0x00499230
class CAckMessage : public CNetworkMessage {
public:
	CAckMessage() : CNetworkMessage(NETWORK_MESSAGE_ACKNOWLEDGEMENT)
	{
		m_payloadCapacity += sizeof(m_packetSequence) + sizeof(m_subpacketSequence);
	}
	void AddData();
	void GetData();

private:
	unsigned short m_packetSequence;    // 0x2c
	unsigned short m_subpacketSequence; // 0x2e

	friend class CWriteSocket;
	friend class CReadSocket;
};

extern CAckMessage* g_pAckMessage;

// SYNTHETIC: LEMBALL 0x00462b80
// CAckMessage::`scalar deleting destructor'

#endif
