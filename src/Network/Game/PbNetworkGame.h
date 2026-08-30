#ifndef LEMBALL_NETWORK_GAME_PBNETWORKGAME_H
#define LEMBALL_NETWORK_GAME_PBNETWORKGAME_H

#include "../../Common.h"
#include "../../Visos/Messaging/NetworkMessage.h" // complete type

// SIZE 0x34
// VTABLE: LEMBALL 0x00498610
class PbNetworkGame : public NetworkMessage {
public:
	PbNetworkGame(Ai* p_arg0);
	virtual void AddData();   // vtable+0x10
	virtual void GetData();   // vtable+0x08
	virtual ~PbNetworkGame(); // vtable+0x14

private:
	Ai* m_ai;                          // 0x2c
	PlayerLemming** m_networkLemmings; // 0x30
};

// SYNTHETIC: LEMBALL 0x00453120
// PbNetworkGame::`scalar deleting destructor'

#endif
