#ifndef LEMBALL_NETWORK_GAME_CPBNETWORKGAME_H
#define LEMBALL_NETWORK_GAME_CPBNETWORKGAME_H

#include "../../Visos/Messaging/CNetworkMessage.h" // complete type

class CAI;
class CPlayerLemming;
// SIZE 0x34
// VTABLE: LEMBALL 0x00498610
class CPBNetworkGame : public CNetworkMessage {
public:
	CPBNetworkGame(CAI* p_arg0);
	virtual void AddData(); // vtable+0x10
	virtual void GetData(); // vtable+0x08

private:
	CAI* m_ai;                          // 0x2c
	CPlayerLemming** m_networkLemmings; // 0x30
};

// SYNTHETIC: LEMBALL 0x00453120
// CPBNetworkGame::`scalar deleting destructor'

#endif
