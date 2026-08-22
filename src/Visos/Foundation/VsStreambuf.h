#ifndef LEMBALL_VISOS_FOUNDATION_VSSTREAMBUF_H
#define LEMBALL_VISOS_FOUNDATION_VSSTREAMBUF_H

#include "../../Common.h"

// SIZE 0x18
// VTABLE: LEMBALL 0x00498888
class VsStreambuf {
public:
	VsStreambuf(char* p_arg0, int p_arg1);
	virtual ~VsStreambuf(); // vtable+0x00
	VsStreambuf();

public:
	char* m_buffer;          // 0x04
	unsigned int m_capacity; // 0x08
	char* m_cursor;          // 0x0c
	unsigned int m_length;   // 0x10
	unsigned int m_tabWidth; // 0x14
};

#endif
