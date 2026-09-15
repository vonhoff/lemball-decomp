#ifndef LEMBALL_VISOS_FOUNDATION_VSOSTREAM_H
#define LEMBALL_VISOS_FOUNDATION_VSOSTREAM_H

#include "../../Common.h"
#include "VsIOs.h" // complete type

// SIZE 0x14c
// VTABLE: LEMBALL 0x00493038
class VsOStream : public virtual VsIOs {
public:
	VsOStream& operator<<(Har4 p_arg0);
	VsOStream& operator<<(Hex p_arg0);
	VsOStream& operator<<(Hex8 p_arg0);
	VsOStream& operator<<(char p_arg0);
#ifdef LEMBALL_OUTLINE_STREAM_HELPERS
	VsOStream& operator<<(unsigned char p_value);
#endif
	VsOStream& operator<<(const char* p_arg0);
	VsOStream& operator<<(const void* p_arg0);
	VsOStream& operator<<(int p_arg0);
	VsOStream& operator<<(long p_arg0);
	VsOStream& operator<<(unsigned long p_arg0);
	VsOStream& operator<<(unsigned int p_arg0);
	VsOStream(VsStreambuf* p_arg0);
	void InternalFormatNum();
	~VsOStream();

public:
	unsigned char m_numberBuffer[0x124]; // 0x04
	char* m_formattedText;               // 0x128
};

VsOStream& operator<<(VsOStream& p_arg0, Rname p_arg1);
VsOStream& operator<<(VsOStream& p_arg0, String& p_arg1);

extern VsOStream* g_pErrorOutput;
extern VsOStream* g_pSysOutput;
extern VsOStream* g_pDebugOutput;

// SYNTHETIC: LEMBALL 0x00407e40
// VsOStream::`scalar deleting destructor'

// GLOBAL: LEMBALL 0x00498898
// VsOStream::`vbtable'

#endif
