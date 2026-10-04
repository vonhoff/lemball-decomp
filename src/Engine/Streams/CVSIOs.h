#ifndef LEMBALL_VISOS_FOUNDATION_CVSIOS_H
#define LEMBALL_VISOS_FOUNDATION_CVSIOS_H

class CVSStreambuf;

enum eVSOStreamFormatFlag {
	VSO_STREAM_LEFT_ADJUST_FLAG = 0x02,
	VSO_STREAM_BASE_FIELD_MASK = 0x8030,
	VSO_STREAM_HEXADECIMAL_BASE_FLAG = 0x40
};

enum eVSORadix {
	VSO_RADIX_DECIMAL = 10,
	VSO_RADIX_HEXADECIMAL = 16
};

// SIZE 0x20
// VTABLE: LEMBALL 0x00493034
class CVSIOs {
public:
	CVSIOs(CVSStreambuf* p_streamBuffer);
	virtual ~CVSIOs(); // vtable+0x00
	CVSIOs();

public:
	int m_state;                  // 0x04
	unsigned int m_flags;         // 0x08
	int m_precision;              // 0x0c
	char m_fill;                  // 0x10
	unsigned int m_width;         // 0x14
	unsigned int m_radix;         // 0x18
	CVSStreambuf* m_streamBuffer; // 0x1c
};

// SYNTHETIC: LEMBALL 0x00407e10
// CVSIOs::`scalar deleting destructor'

#endif
