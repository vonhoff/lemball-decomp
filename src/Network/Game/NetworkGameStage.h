#ifndef LEMBALL_NETWORK_GAME_NETWORKGAMESTAGE_H
#define LEMBALL_NETWORK_GAME_NETWORKGAMESTAGE_H

#include "../../Common.h"
#include "../../Visos/Messaging/NetworkMessage.h" // complete type

// SIZE 0x30
// VTABLE: LEMBALL 0x00498558
class NetworkGameStage : public NetworkMessage {
public:
	NetworkGameStage();
	virtual void AddData();      // vtable+0x10
	virtual void GetData();      // vtable+0x08
	virtual ~NetworkGameStage(); // vtable+0x14

private:
	unsigned int m_stage; // 0x2c
};

// SYNTHETIC: LEMBALL 0x00452e60
// NetworkGameStage::`scalar deleting destructor'


#endif

