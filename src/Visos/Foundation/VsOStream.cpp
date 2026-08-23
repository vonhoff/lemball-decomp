#include "VsOStream.h"

#include "String.h"
#include "VsDebugStreambuf.h"
#include "VsString.h"

#include <string.h>

#pragma intrinsic(memset, memcpy, strlen)

// 68K 0x10213d5e __ct__10CVSOStreamFP12CVSStreambuf
// FUNCTION: LEMBALL 0x00458450
VsOStream::VsOStream(VsStreambuf* p_arg0) : VsIOs(p_arg0)
{
}

// 68K 0x10213dc6 __dt__10CVSOStreamFv
// FUNCTION: LEMBALL 0x004584a0
VsOStream::~VsOStream()
{
}

// 68K 0x10213e2a _FormatNum__10CVSOStreamFv
// FUNCTION: LEMBALL 0x004584c0
void VsOStream::FormatNum()
{
	if (m_width != 0) {
		bool isNeg = (m_numberBuffer[0] == '-');
		int signLen = isNeg ? 1 : 0;
		int len = strlen((char*) m_numberBuffer);

		int width = m_width;
		char fill = m_fill;

		memset(m_numberBuffer + 0x21, fill, width);
		m_numberBuffer[0x21 + width] = '\0';

		if (isNeg) {
			m_numberBuffer[0x21] = '-';
		}

		char* dst;
		char* src;
		if (m_flags & 2) {
			dst = (char*) m_numberBuffer + 0x21 + signLen;
			src = (char*) m_numberBuffer + signLen;
		}
		else {
			int srcOffset = signLen;
			if (len >= width) {
				srcOffset = len - width;
			}
			int dstOffset = signLen;
			if (len < width) {
				dstOffset = signLen - len + width;
			}
			src = (char*) m_numberBuffer + srcOffset;
			dst = (char*) m_numberBuffer + 0x21 + dstOffset;
		}

		memcpy(dst, src, width - signLen);
		m_formattedText = (char*) (m_numberBuffer + 0x21);
	}
	else {
		m_formattedText = (char*) m_numberBuffer;
	}
}

// 68K 0x10213f2c __ls__10CVSOStreamFPCc
// FUNCTION: LEMBALL 0x004585b0
VsOStream& VsOStream::operator<<(const char* p_arg0)
{
	((VsDebugStreambuf*) m_streamBuffer)->Sputs((char*) p_arg0);
	return *this;
}

// 68K 0x10213f6a __ls__10CVSOStreamFc
// FUNCTION: LEMBALL 0x004585d0
VsOStream& VsOStream::operator<<(char p_arg0)
{
	((VsDebugStreambuf*) m_streamBuffer)->Sputc(p_arg0);
	return *this;
}

// 68K 0x10213fa6 __ls__10CVSOStreamFl
// FUNCTION: LEMBALL 0x004585f0
VsOStream& VsOStream::operator<<(long p_arg0)
{
	VsLtoa(p_arg0, (char*) m_numberBuffer, m_radix);
	FormatNum();
	return *this << m_formattedText;
}

// 68K 0x10213ff6 __ls__10CVSOStreamFi
// FUNCTION: LEMBALL 0x00458630
VsOStream& VsOStream::operator<<(int p_arg0)
{
	VsLtoa(p_arg0, (char*) m_numberBuffer, m_radix);
	FormatNum();
	return *this << m_formattedText;
}

// 68K 0x10214046 __ls__10CVSOStreamFPCv
// FUNCTION: LEMBALL 0x00458670
VsOStream& VsOStream::operator<<(const void* p_arg0)
{
	if (p_arg0 == NULL) {
		return *this << "(null)";
	}
	VsULtoa((unsigned long) p_arg0, (char*) m_numberBuffer, 16);
	FormatNum();
	return *this << "0x" << m_formattedText;
}

// 68K 0x102140ba __ls__10CVSOStreamFUl
// FUNCTION: LEMBALL 0x004586e0
VsOStream& VsOStream::operator<<(unsigned long p_arg0)
{
	VsULtoa(p_arg0, (char*) m_numberBuffer, m_radix);
	FormatNum();
	return *this << m_formattedText;
}

// 68K 0x1021410a __ls__10CVSOStreamF3HEX
// FUNCTION: LEMBALL 0x00458780
VsOStream& VsOStream::operator<<(Hex p_arg0)
{
	unsigned int oldFlags = m_flags;
	m_flags = (oldFlags & ~0x8030) | 0x40;
	unsigned int oldRadix = m_radix;
	m_radix = 16;
	*this << (unsigned long) p_arg0.m_value;

	m_radix = oldRadix;
	m_flags = oldFlags;
	return *this;
}

// 68K 0x10214186 __ls__10CVSOStreamF4HEX8
// FUNCTION: LEMBALL 0x004589c0
VsOStream& VsOStream::operator<<(Hex8 p_arg0)
{
	char oldFill = m_fill;
	m_fill = '0';
	unsigned int oldWidth = m_width;
	m_width = 8;
	unsigned int oldFlags = m_flags;
	m_flags = (oldFlags & ~0x8030) | 0x40;
	unsigned int oldRadix = m_radix;
	m_radix = 16;
	*this << (unsigned long) p_arg0.m_value;

	m_width = oldWidth;
	m_radix = oldRadix;
	m_fill = oldFill;
	m_flags = oldFlags;
	return *this;
}

// 68K 0x10214240 __ls__10CVSOStreamF5CHAR4
// FUNCTION: LEMBALL 0x00458d40
VsOStream& VsOStream::operator<<(Har4 p_arg0)
{
	unsigned char shift = 0x18;
	int i = 3;
	unsigned long value = p_arg0.m_value;
	do {
		m_streamBuffer->Sputc((char) (value >> shift));
		shift = (unsigned char) ((signed char) shift - 8);
	} while (--i >= 0);
	return *this;
}

// 68K 0x10201618 __ls__FR10CVSOStream5RNAME
// FUNCTION: LEMBALL 0x0045bad0
VsOStream& operator<<(VsOStream& p_arg0, Rname p_arg1)
{
	p_arg0 << (int) p_arg1.m_value;
	return p_arg0;
}

// GLOBAL: LEMBALL 0x004a93a8
VsOStream* g_pErrorOutput;

// GLOBAL: LEMBALL 0x004a97b8
VsOStream* g_pSysOutput;

// GLOBAL: LEMBALL 0x004a97bc
VsOStream* g_pDebugOutput;
