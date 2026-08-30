#ifndef LEMBALL_VISOS_MESSAGING_HEADERS_H
#define LEMBALL_VISOS_MESSAGING_HEADERS_H

#include "../../Common.h"
#include "NetworkMessage.h" // complete type

// SIZE 0x3c
// VTABLE: LEMBALL 0x0049a4c0
class Headers : public NetworkMessage {
public:
	Headers(int p_arg0);
	virtual void AddData(); // vtable+0x10
	virtual void GetData(); // vtable+0x08
	virtual ~Headers();     // vtable+0x14

private:
	HeaderMessage* m_headers;    // 0x2c
	unsigned short* m_sequences; // 0x30
	int m_count;                 // 0x34
	int m_currentIndex;          // 0x38
};

// SYNTHETIC: LEMBALL 0x0047b7d0
// Headers::`scalar deleting destructor'

#endif
