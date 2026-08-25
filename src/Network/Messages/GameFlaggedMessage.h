#ifndef LEMBALL_NETWORK_MESSAGES_GAMEFLAGGEDMESSAGE_H
#define LEMBALL_NETWORK_MESSAGES_GAMEFLAGGEDMESSAGE_H

#include "../../Common.h"
#include "../../Visos/Messaging/NetworkMessage.h" // complete type

// SIZE 0x30
// VTABLE: LEMBALL 0x00498598
class GameFlaggedMessage : public NetworkMessage {
public:
	GameFlaggedMessage(unsigned long p_arg0);
	virtual void AddData();        // vtable+0x10
	virtual void GetData();        // vtable+0x08
	virtual ~GameFlaggedMessage(); // vtable+0x14
	GameFlaggedMessage();

private:
	unsigned int m_flag; // 0x2c

	friend class NetworkOptionsProc;
};

#endif
