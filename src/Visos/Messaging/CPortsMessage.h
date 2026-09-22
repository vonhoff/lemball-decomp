#ifndef LEMBALL_VISOS_MESSAGING_CPORTSMESSAGE_H
#define LEMBALL_VISOS_MESSAGING_CPORTSMESSAGE_H

#include "CNetworkMessage.h" // complete type

// SIZE 0x30
// VTABLE: LEMBALL 0x0049a4d8
class CPortsMessage : public CNetworkMessage {
public:
	CPortsMessage();
	bool AnyUsed();
	virtual void AddData();                            // vtable+0x10
	virtual void GetData();                            // vtable+0x08
	virtual ~CPortsMessage() { delete[] m_useCounts; } // vtable+0x14

private:
	friend class CFileBroadcast;
	unsigned char* m_useCounts; // 0x2c
};

// SYNTHETIC: LEMBALL 0x0047b890
// CPortsMessage::`scalar deleting destructor'

#endif
