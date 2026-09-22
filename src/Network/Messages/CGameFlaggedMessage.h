#ifndef LEMBALL_NETWORK_MESSAGES_CGAMEFLAGGEDMESSAGE_H
#define LEMBALL_NETWORK_MESSAGES_CGAMEFLAGGEDMESSAGE_H

#include "../../Visos/Messaging/CNetworkMessage.h" // complete type

// SIZE 0x30
// VTABLE: LEMBALL 0x00498598
class CGameFlaggedMessage : public CNetworkMessage {
public:
	CGameFlaggedMessage(unsigned long p_arg0);
	virtual void AddData(); // vtable+0x10
	virtual void GetData(); // vtable+0x08
	CGameFlaggedMessage();

private:
	unsigned int m_flag; // 0x2c

	friend class CNetworkManager;
	friend class CNetworkOptionsProc;
};

// SYNTHETIC: LEMBALL 0x00452e90
// CGameFlaggedMessage::`scalar deleting destructor'

#endif
