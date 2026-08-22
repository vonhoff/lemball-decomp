#ifndef LEMBALL_SCAFFOLD_VISOS_NETWORK_BASESOCKET_H
#define LEMBALL_SCAFFOLD_VISOS_NETWORK_BASESOCKET_H

#include "../../Common.h"
#include "../Messaging/NetworkMessage.h" // complete type

// SIZE 0x38
// VTABLE: LEMBALL 0x00498f40
class BaseSocket : public NetworkMessage {
public:
	BaseSocket();
	virtual ~BaseSocket(); // vtable+0x14
	void AddData();
	void GetData();

private:
	BasePacketHeader* m_packetHeader;      // 0x2c
	unsigned short m_criticalSequence;     // 0x30
	unsigned short m_nonCriticalSequence;  // 0x32
	unsigned short m_receiveSequence;      // 0x34
	unsigned short m_multiMessageSequence; // 0x36
};

#endif
