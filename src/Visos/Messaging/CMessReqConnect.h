#ifndef LEMBALL_VISOS_MESSAGING_CMESSREQCONNECT_H
#define LEMBALL_VISOS_MESSAGING_CMESSREQCONNECT_H

#include "CBroadcastMessage.h" // complete type

// SIZE 0x3c
// VTABLE: LEMBALL 0x00498eb8
class CMessReqConnect : public CBroadcastMessage {
public:
	CMessReqConnect(const char* p_arg0);
	void AddData();
	void GetData();

	friend class CBaseNetwork;
	friend class CBroadcast;

private:
	unsigned short m_requestedPort;  // 0x30
	unsigned char* m_connectionData; // 0x34
	char* m_peerName;                // 0x38
};

extern CMessReqConnect* g_pMessReqConnect;

// SYNTHETIC: LEMBALL 0x00462830
// CMessReqConnect::`scalar deleting destructor'

#endif
