#ifndef LEMBALL_NETWORK_GAME_NETWORKGAMESTAGE_H
#define LEMBALL_NETWORK_GAME_NETWORKGAMESTAGE_H

#include "../../Visos/Messaging/NetworkMessage.h" // complete type

class NetworkManager;
// SIZE 0x30
// VTABLE: LEMBALL 0x00498558
class NetworkGameStage : public NetworkMessage {
public:
	NetworkGameStage();
	virtual void AddData(); // vtable+0x10
	virtual void GetData(); // vtable+0x08

private:
	unsigned int m_stage; // 0x2c

	friend class NetworkManager;
};

// SYNTHETIC: LEMBALL 0x00452e60
// NetworkGameStage::`scalar deleting destructor'

#endif
