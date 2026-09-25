#ifndef LEMBALL_VISOS_NETWORK_COPENCOUNT_H
#define LEMBALL_VISOS_NETWORK_COPENCOUNT_H

#include "../Messaging/CNetworkMessage.h"

// SIZE 0x30
// VTABLE: LEMBALL 0x0049a848
class COpenCount : public CNetworkMessage {
public:
	COpenCount() : CNetworkMessage(0)
	{
		m_payloadCapacity += 2;
		m_openCount = 0;
	}
	virtual void AddData(); // vtable+0x10
	virtual void GetData(); // vtable+0x08
	friend class CFileOpenManagement;
	friend class CFileConnect;
	friend class CFileBroadcast;

private:
	unsigned short m_openCount;  // 0x2c
	unsigned short m_reserved2e; // 0x2e
};

// SYNTHETIC: LEMBALL 0x0047b970
// COpenCount::`scalar deleting destructor'

#endif
