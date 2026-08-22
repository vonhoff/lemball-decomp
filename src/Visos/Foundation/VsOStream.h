#ifndef LEMBALL_VISOS_FOUNDATION_VSOSTREAM_H
#define LEMBALL_VISOS_FOUNDATION_VSOSTREAM_H

#include "../../Common.h"
#include "VsIOs.h" // complete type

// SIZE 0x14c
class VsOStream : public virtual VsIOs {
public:
	VsOStream& operator<<(Har4 p_arg0);
	VsOStream& operator<<(Hex p_arg0);
	VsOStream& operator<<(Hex8 p_arg0);
	VsOStream& operator<<(char p_arg0);
	VsOStream& operator<<(const char* p_arg0);
	VsOStream& operator<<(const void* p_arg0);
	VsOStream& operator<<(int p_arg0);
	VsOStream& operator<<(long p_arg0);
	VsOStream& operator<<(unsigned long p_arg0);
	VsOStream(VsStreambuf* p_arg0);
	void FormatNum();
	~VsOStream();

public:
	unsigned char m_numberBuffer[0x124]; // 0x04
	char* m_formattedText;               // 0x128
};

VsOStream& operator<<(VsOStream& p_arg0, Rname p_arg1);
VsOStream& operator<<(VsOStream& p_arg0, String& p_arg1);

// Confirmed class-scoped globals.
extern VsOStream* g_pErrorOutput;
extern VsOStream* g_pSysOutput;
#endif
