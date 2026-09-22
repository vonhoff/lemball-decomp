#ifndef LEMBALL_VISOS_FOUNDATION_CVSIOS_H
#define LEMBALL_VISOS_FOUNDATION_CVSIOS_H

class CVsStreambuf;

// SIZE 0x20
// VTABLE: LEMBALL 0x00493034
class CVsIOs {
public:
	CVsIOs(CVsStreambuf* p_arg0);
	virtual ~CVsIOs(); // vtable+0x00
	CVsIOs();

public:
	int m_state;                  // 0x04
	unsigned int m_flags;         // 0x08
	int m_precision;              // 0x0c
	char m_fill;                  // 0x10
	unsigned int m_width;         // 0x14
	unsigned int m_radix;         // 0x18
	CVsStreambuf* m_streamBuffer; // 0x1c
};

extern char g_szCurrentDirectory[256];
extern char g_szSourceDiskPath[256];

// SYNTHETIC: LEMBALL 0x00407e10
// CVsIOs::`scalar deleting destructor'

#endif
