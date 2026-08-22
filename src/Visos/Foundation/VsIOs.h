#ifndef LEMBALL_VISOS_FOUNDATION_VSIOS_H
#define LEMBALL_VISOS_FOUNDATION_VSIOS_H

#include "../../Common.h"

// SIZE 0x20
// VTABLE: LEMBALL 0x00493034
class VsIOs {
public:
	VsIOs(VsStreambuf* p_arg0);
	virtual ~VsIOs(); // vtable+0x00
	VsIOs();

private:
	unsigned int m_flags;        // 0x08
	char m_fill;                 // 0x10
	unsigned int m_width;        // 0x14
	unsigned int m_radix;        // 0x18
	VsStreambuf* m_streamBuffer; // 0x1c
};

#endif
