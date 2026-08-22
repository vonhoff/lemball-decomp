#ifndef LEMBALL_SCAFFOLD_VISOS_MESSAGING_MESSGOCONNECT_H
#define LEMBALL_SCAFFOLD_VISOS_MESSAGING_MESSGOCONNECT_H

#include "../../Common.h"
#include "BroadcastMessage.h" // complete type

// SIZE 0x38
// VTABLE: LEMBALL 0x00498f00
class MessGoConnect : public BroadcastMessage {
public:
	MessGoConnect(const char* p_arg0);
	void AddData();
	void GetData();
	~MessGoConnect();

private:
	unsigned short m_assignedPort; // 0x30
	unsigned int m_connectionId;   // 0x34
};

// Confirmed class-scoped globals.
extern MessGoConnect* g_pMessGOConnect;
#endif
