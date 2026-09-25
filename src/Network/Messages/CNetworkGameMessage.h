#ifndef LEMBALL_NETWORK_MESSAGES_CNETWORKGAMEMESSAGE_H
#define LEMBALL_NETWORK_MESSAGES_CNETWORKGAMEMESSAGE_H

#include "../../Visos/Messaging/CNetworkMessage.h"

// SIZE 0x50
// VTABLE: LEMBALL 0x00498578
class CNetworkGameMessage : public CNetworkMessage {
public:
	CNetworkGameMessage();
	virtual void AddData(); // vtable+0x10
	virtual void GetData(); // vtable+0x08

	friend class CNetworkOptionsDrawer;
	friend class CNetworkManager;
	friend class CSuccFailDrawer;

private:
	char m_gameName[9];       // 0x2c
	char m_peerName[21];      // 0x35
	unsigned short m_padding; // 0x4a
	unsigned int m_valid;     // 0x4c
};

// SYNTHETIC: LEMBALL 0x00452f50
// CNetworkGameMessage::`vector deleting destructor'

// SYNTHETIC: LEMBALL 0x00452fd0
// CNetworkGameMessage::~CNetworkGameMessage

#endif
