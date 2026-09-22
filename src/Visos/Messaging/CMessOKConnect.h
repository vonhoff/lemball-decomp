#ifndef LEMBALL_VISOS_MESSAGING_CMESSOKCONNECT_H
#define LEMBALL_VISOS_MESSAGING_CMESSOKCONNECT_H

#include "CBroadcastMessage.h" // complete type

// SIZE 0x38
// VTABLE: LEMBALL 0x00498ee8
class CMessOKConnect : public CBroadcastMessage {
public:
	CMessOKConnect(const char* p_arg0);
	void AddData();
	void GetData();

	friend class CBaseNetwork;

private:
	unsigned short m_assignedPort; // 0x30
	unsigned int m_connectionId;   // 0x34
};

extern CMessOKConnect* g_pMessOKConnect;

// SYNTHETIC: LEMBALL 0x00462870
// CMessOKConnect::`scalar deleting destructor'

#endif
