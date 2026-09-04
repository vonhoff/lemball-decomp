#ifndef LEMBALL_VISOS_MESSAGING_MESSREQCONNECT_H
#define LEMBALL_VISOS_MESSAGING_MESSREQCONNECT_H

#include "../../Common.h"
#include "BroadcastMessage.h" // complete type

// SIZE 0x3c
// VTABLE: LEMBALL 0x00498eb8
class MessReqConnect : public BroadcastMessage {
public:
	MessReqConnect(const char* p_arg0);
	void AddData();
	void GetData();
	~MessReqConnect();

	friend class BaseNetwork;
	friend class Broadcast;

private:
	unsigned short m_requestedPort;  // 0x30
	unsigned char* m_connectionData; // 0x34
	char* m_peerName;                // 0x38
};

extern MessReqConnect* g_pMessReqConnect;

// SYNTHETIC: LEMBALL 0x00462830
// MessReqConnect::`scalar deleting destructor'

#endif
