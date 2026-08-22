#ifndef LEMBALL_VISOS_MESSAGING_HEADERMESSAGE_H
#define LEMBALL_VISOS_MESSAGING_HEADERMESSAGE_H

#include "../../Common.h"
#include "NetworkMessage.h" // complete type

// SIZE 0x60
// VTABLE: LEMBALL 0x0049a4a8
class HeaderMessage : public NetworkMessage {
public:
	HeaderMessage();
	virtual void AddData();   // vtable+0x10
	virtual void GetData();   // vtable+0x08
	virtual ~HeaderMessage(); // vtable+0x14

private:
	unsigned short m_sequence;         // 0x2c
	unsigned short m_mirroredSequence; // 0x2e
	unsigned int m_headerValue;        // 0x30
	char m_text0[21];                  // 0x34
	char m_text1[21];                  // 0x49
	undefined2 m_padding;              // 0x5e
};

#endif
