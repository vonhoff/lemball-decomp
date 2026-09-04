#ifndef LEMBALL_VISOS_NETWORK_BASESOCKET_H
#define LEMBALL_VISOS_NETWORK_BASESOCKET_H

#include "../../Common.h"
#include "../Messaging/NetworkMessage.h" // complete type

// SIZE 0x44
// VTABLE: LEMBALL 0x00498f40
class BaseSocket : public NetworkMessage {
public:
	BaseSocket();
	void AddData();
	void GetData();

	friend class BaseObjectManager;

protected:
	BasePacketHeader* m_packetHeader;      // 0x2c
	unsigned short m_criticalSequence;     // 0x30
	unsigned short m_nonCriticalSequence;  // 0x32
	unsigned short m_receiveSequence;      // 0x34
	unsigned short m_multiMessageSequence; // 0x36
	undefined4 m_unknown38;                // 0x38
	union {
		unsigned int m_lastReceiveTime; // 0x3c
		unsigned int m_lastSendTime;    // 0x3c
	};
	undefined4 m_unknown40; // 0x40
};

// SYNTHETIC: LEMBALL 0x00462900
// BaseSocket::`scalar deleting destructor'

#endif
