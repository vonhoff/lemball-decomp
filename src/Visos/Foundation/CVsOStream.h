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
	CVsOStream& operator<<(CHAR4 p_arg0);
	CVsOStream& operator<<(Hex p_arg0);
	CVsOStream& operator<<(Hex8 p_arg0);
	CVsOStream& operator<<(char p_arg0);
	CVsOStream& operator<<(unsigned char p_value);
	CVsOStream& operator<<(const char* p_arg0);
	CVsOStream& operator<<(const void* p_arg0);
	CVsOStream& operator<<(int p_arg0);
	CVsOStream& operator<<(long p_arg0);
	CVsOStream& operator<<(unsigned long p_arg0);
	CVsOStream& operator<<(unsigned int p_arg0);
	CVsOStream(CVsStreambuf* p_arg0);
	void InternalFormatNum();
	~CVsOStream();

public:
	unsigned char m_numberBuffer[0x124]; // 0x04
	char* m_formattedText;               // 0x128
};

CVsOStream& operator<<(CVsOStream& p_arg0, Rname p_arg1);
CVsOStream& operator<<(CVsOStream& p_arg0, CString& p_arg1);

extern CVsOStream* g_pErrorOutput;
extern CVsOStream* g_pSysOutput;
extern CVsOStream* g_pDebugOutput;

// SYNTHETIC: LEMBALL 0x00407e40
// CVsOStream::`scalar deleting destructor'

// GLOBAL: LEMBALL 0x00498898
// CVsOStream::`vbtable'

#endif
