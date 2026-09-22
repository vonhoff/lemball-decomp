#ifndef LEMBALL_VISOS_MESSAGING_CMESSFAILEDCONNECT_H
#define LEMBALL_VISOS_MESSAGING_CMESSFAILEDCONNECT_H

#include "CBroadcastMessage.h" // complete type

// SIZE 0x34
// VTABLE: LEMBALL 0x00498f18
class CMessFailedConnect : public CBroadcastMessage {
public:
	CMessFailedConnect(const char* p_arg0);
	void AddData();
	void GetData();

	friend class CBaseNetwork;

private:
	char* m_failureReason; // 0x30
};

extern CMessFailedConnect* g_pMessFAILEDConnect;

// SYNTHETIC: LEMBALL 0x004628b0
// CMessFailedConnect::`scalar deleting destructor'

#endif
