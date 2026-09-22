#ifndef LEMBALL_VISOS_MESSAGING_CHEADERS_H
#define LEMBALL_VISOS_MESSAGING_CHEADERS_H

#include "CNetworkMessage.h" // complete type

class CHeaderMessage;

// SIZE 0x3c
// VTABLE: LEMBALL 0x0049a4c0
class CHeaders : public CNetworkMessage {
public:
	CHeaders(int p_arg0);
	CHeaderMessage* GetNextHeaderBySequence();
	virtual void AddData(); // vtable+0x10
	virtual void GetData(); // vtable+0x08
	virtual ~CHeaders();    // vtable+0x14

private:
	friend class CFileBroadcast;
	friend class CFileReadSocket;
	friend class CFileWriteSocket;
	CHeaderMessage* m_headers;   // 0x2c
	unsigned short* m_sequences; // 0x30
	int m_count;                 // 0x34
	int m_currentIndex;          // 0x38
};

// SYNTHETIC: LEMBALL 0x0047b7d0
// CHeaders::`scalar deleting destructor'

#endif
