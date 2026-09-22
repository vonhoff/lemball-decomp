#ifndef LEMBALL_NETWORK_GAME_CPBNETWORKGAME_H
#define LEMBALL_NETWORK_GAME_CPBNETWORKGAME_H

#include "../../Visos/Messaging/CNetworkMessage.h" // complete type

class CAi;
class CPlayerLemming;
// SIZE 0x34
// VTABLE: LEMBALL 0x00498610
class CPbNetworkGame : public CNetworkMessage {
public:
	CPbNetworkGame(CAi* p_arg0);
	virtual void AddData(); // vtable+0x10
	virtual void GetData(); // vtable+0x08

private:
	CAi* m_ai;                          // 0x2c
	CPlayerLemming** m_networkLemmings; // 0x30
};

// SYNTHETIC: LEMBALL 0x00453120
// CPbNetworkGame::`scalar deleting destructor'

#endif
