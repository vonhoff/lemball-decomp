#ifndef LEMBALL_VISOS_MESSAGING_MESSFAILEDCONNECT_H
#define LEMBALL_VISOS_MESSAGING_MESSFAILEDCONNECT_H

#include "../../Common.h"
#include "BroadcastMessage.h" // complete type

// SIZE 0x34
// VTABLE: LEMBALL 0x00498f18
class MessFailedConnect : public BroadcastMessage {
public:
	MessFailedConnect(const char* p_arg0);
	void AddData();
	void GetData();
	~MessFailedConnect();

	friend class BaseNetwork;

private:
	char* m_failureReason; // 0x30
};

extern MessFailedConnect* g_pMessFAILEDConnect;

// SYNTHETIC: LEMBALL 0x004628b0
// MessFailedConnect::`scalar deleting destructor'

#endif
