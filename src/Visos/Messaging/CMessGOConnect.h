#ifndef LEMBALL_VISOS_MESSAGING_CMESSGOCONNECT_H
#define LEMBALL_VISOS_MESSAGING_CMESSGOCONNECT_H

#include "CBroadcastMessage.h" // complete type

// SIZE 0x38
// VTABLE: LEMBALL 0x00498f00
class CMessGOConnect : public CBroadcastMessage {
public:
	CMessGOConnect(const char* p_arg0);
	void AddData();
	void GetData();

	friend class CBaseNetwork;

private:
	unsigned short m_assignedPort; // 0x30
	unsigned int m_connectionId;   // 0x34
};

extern CMessGOConnect* g_pMessGOConnect;

// SYNTHETIC: LEMBALL 0x00462890
// CMessGOConnect::`scalar deleting destructor'

#endif
