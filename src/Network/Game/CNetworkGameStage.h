#ifndef LEMBALL_NETWORK_GAME_CNETWORKGAMESTAGE_H
#define LEMBALL_NETWORK_GAME_CNETWORKGAMESTAGE_H

#include "../../Visos/Messaging/CNetworkMessage.h" // complete type

// SIZE 0x30
// VTABLE: LEMBALL 0x00498558
class CNetworkGameStage : public CNetworkMessage {
public:
	CNetworkGameStage();
	virtual void AddData(); // vtable+0x10
	virtual void GetData(); // vtable+0x08

private:
	unsigned int m_stage; // 0x2c

	friend class CNetworkManager;
};

// SYNTHETIC: LEMBALL 0x00452e60
// CNetworkGameStage::`scalar deleting destructor'

#endif
