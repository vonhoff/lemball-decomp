#ifndef LEMBALL_VISOS_MESSAGING_CHEADERMESSAGE_H
#define LEMBALL_VISOS_MESSAGING_CHEADERMESSAGE_H

#include "CNetworkMessage.h" // complete type

// SIZE 0x60
// VTABLE: LEMBALL 0x0049a4a8
class CHeaderMessage : public CNetworkMessage {
public:
	CHeaderMessage();
	virtual void AddData(); // vtable+0x10
	virtual void GetData(); // vtable+0x08

private:
	friend class CHeaders;
	friend class CFileReadSocket;
	friend class CFileWriteSocket;
	unsigned short m_sequence;         // 0x2c
	unsigned short m_mirroredSequence; // 0x2e
	unsigned long m_headerValue;       // 0x30
	char m_text0[21];                  // 0x34
	char m_text1[21];                  // 0x49
	unsigned short m_padding;          // 0x5e
};

// SYNTHETIC: LEMBALL 0x0047b7f0
// CHeaderMessage::`vector deleting destructor'

// SYNTHETIC: LEMBALL 0x0047b860
// CHeaderMessage::~CHeaderMessage

#endif
