#ifndef LEMBALL_VISOS_MESSAGING_MESSOKCONNECT_H
#define LEMBALL_VISOS_MESSAGING_MESSOKCONNECT_H

#include "../../Common.h"
#include "BroadcastMessage.h" // complete type

// SIZE 0x38
// VTABLE: LEMBALL 0x00498ee8
class MessOkConnect : public BroadcastMessage {
public:
	MessOkConnect(const char* p_arg0);
	void AddData();
	void GetData();
	~MessOkConnect();

private:
	unsigned short m_assignedPort; // 0x30
	unsigned int m_connectionId;   // 0x34
};

extern MessOkConnect* g_pMessOKConnect;

// SYNTHETIC: LEMBALL 0x00462870
// MessOkConnect::`scalar deleting destructor'

#endif
