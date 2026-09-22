#ifndef LEMBALL_VISOS_FOUNDATION_CVSOSTREAM_H
#define LEMBALL_VISOS_FOUNDATION_CVSOSTREAM_H

#include "CVSIOs.h" // complete type
class CVSStreambuf;

class CString;

// SIZE 0x04
struct CHAR4 {
	unsigned int m_value;
	CHAR4(unsigned int v = 0) : m_value(v) {}
};

// SIZE 0x04
struct Hex {
	unsigned int m_value;
	Hex(unsigned int v = 0) : m_value(v) {}
};

// SIZE 0x04
struct Hex8 {
	unsigned int m_value;
	Hex8(unsigned int v = 0) : m_value(v) {}
};

// SIZE 0x04
struct Rname {
	unsigned int m_value;
	Rname(unsigned int v = 0) : m_value(v) {}
};

// SIZE 0x14c
// VTABLE: LEMBALL 0x00493038
class CVSOStream : public virtual CVSIOs {
public:
	CVSOStream& operator<<(CHAR4 p_value);
	CVSOStream& operator<<(Hex p_value);
	CVSOStream& operator<<(Hex8 p_value);
	CVSOStream& operator<<(char p_character);
	CVSOStream& operator<<(unsigned char p_value);
	CVSOStream& operator<<(const char* p_text);
	CVSOStream& operator<<(const void* p_pointer);
	CVSOStream& operator<<(int p_value);
	CVSOStream& operator<<(long p_value);
	CVSOStream& operator<<(unsigned long p_value);
	CVSOStream& operator<<(unsigned int p_value);
	CVSOStream(CVSStreambuf* p_streamBuffer);
	void _FormatNum();
	~CVSOStream();

public:
	unsigned char m_numberBuffer[0x124]; // 0x04
	char* m_formattedText;               // 0x128
};

CVSOStream& operator<<(CVSOStream& p_stream, Rname p_resourceName);
CVSOStream& operator<<(CVSOStream& p_stream, CString& p_string);

extern CVSOStream* g_pErrorOutput;
extern CVSOStream* g_pSysOutput;
extern CVSOStream* g_pDebugOutput;

// SYNTHETIC: LEMBALL 0x00407e40
// CVSOStream::`scalar deleting destructor'

// GLOBAL: LEMBALL 0x00498898
// CVSOStream::`vbtable'

#endif
