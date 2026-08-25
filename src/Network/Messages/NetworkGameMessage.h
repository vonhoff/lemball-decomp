#ifndef LEMBALL_NETWORK_MESSAGES_NETWORKGAMEMESSAGE_H
#define LEMBALL_NETWORK_MESSAGES_NETWORKGAMEMESSAGE_H

#include "../../Common.h"
#include "../../Visos/Messaging/NetworkMessage.h" // complete type

// SIZE 0x50
// VTABLE: LEMBALL 0x00498578
class NetworkGameMessage : public NetworkMessage {
public:
	NetworkGameMessage();
	virtual void AddData();        // vtable+0x10
	virtual void GetData();        // vtable+0x08
	virtual ~NetworkGameMessage(); // vtable+0x14

	friend class NetworkOptionsDrawer;

private:
	char m_gameName[9];   // 0x2c
	char m_peerName[21];  // 0x35
	undefined2 m_padding; // 0x4a
	unsigned int m_valid; // 0x4c
};

#endif
