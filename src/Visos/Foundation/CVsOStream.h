#ifndef LEMBALL_VISOS_FOUNDATION_CVSOSTREAM_H
#define LEMBALL_VISOS_FOUNDATION_CVSOSTREAM_H

#include "CVsIOs.h" // complete type
class CVsStreambuf;

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
class CVsOStream : public virtual CVsIOs {
public:
	CVsOStream& operator<<(CHAR4 p_value);
	CVsOStream& operator<<(Hex p_value);
	CVsOStream& operator<<(Hex8 p_value);
	CVsOStream& operator<<(char p_character);
	CVsOStream& operator<<(unsigned char p_value);
	CVsOStream& operator<<(const char* p_text);
	CVsOStream& operator<<(const void* p_pointer);
	CVsOStream& operator<<(int p_value);
	CVsOStream& operator<<(long p_value);
	CVsOStream& operator<<(unsigned long p_value);
	CVsOStream& operator<<(unsigned int p_value);
	CVsOStream(CVsStreambuf* p_streamBuffer);
	void InternalFormatNum();
	~CVsOStream();

public:
	unsigned char m_numberBuffer[0x124]; // 0x04
	char* m_formattedText;               // 0x128
};

CVsOStream& operator<<(CVsOStream& p_stream, Rname p_resourceName);
CVsOStream& operator<<(CVsOStream& p_stream, CString& p_string);

extern CVsOStream* g_pErrorOutput;
extern CVsOStream* g_pSysOutput;
extern CVsOStream* g_pDebugOutput;

// SYNTHETIC: LEMBALL 0x00407e40
// CVsOStream::`scalar deleting destructor'

// GLOBAL: LEMBALL 0x00498898
// CVsOStream::`vbtable'

#endif
