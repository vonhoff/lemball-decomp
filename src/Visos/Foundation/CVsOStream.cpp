#include "CVsOStream.h"

#include "CVsDebugStreambuf.h"
#include "Visos/Foundation/CVsIOs.h"
#include "Visos/Foundation/CVsStreambuf.h"
#include "VsString.h"

#include <string.h>

#pragma intrinsic(memset, memcpy, strlen)

// FUNCTION: LEMBALL 0x00458450
CVsOStream::CVsOStream(CVsStreambuf* p_arg0) : CVsIOs(p_arg0)
{
}

// FUNCTION: LEMBALL 0x004584a0
CVsOStream::~CVsOStream()
{
}

// FUNCTION: LEMBALL 0x004584c0
void CVsOStream::InternalFormatNum()
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
			int srcOffset;
			if (len < width) {
				srcOffset = signLen;
			}
			else {
				srcOffset = len - width;
			}
			int dstOffset;
			if (len >= width) {
				dstOffset = signLen;
			}
			else {
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

// FUNCTION: LEMBALL 0x004585b0
CVsOStream& CVsOStream::operator<<(const char* p_arg0)
{
	((CVsDebugStreambuf*) m_streamBuffer)->Sputs((char*) p_arg0);
	return *this;
}

// FUNCTION: LEMBALL 0x004585d0
CVsOStream& CVsOStream::operator<<(char p_arg0)
{
	((CVsDebugStreambuf*) m_streamBuffer)->Sputc(p_arg0);
	return *this;
}

// FUNCTION: LEMBALL 0x004585f0
CVsOStream& CVsOStream::operator<<(long p_arg0)
{
	VsLtoa(p_arg0, (char*) m_numberBuffer, m_radix);
	InternalFormatNum();
	return *this << m_formattedText;
}

// FUNCTION: LEMBALL 0x00458630
CVsOStream& CVsOStream::operator<<(int p_arg0)
{
	VsLtoa(p_arg0, (char*) m_numberBuffer, m_radix);
	InternalFormatNum();
	return *this << m_formattedText;
}

// FUNCTION: LEMBALL 0x00458670
CVsOStream& CVsOStream::operator<<(const void* p_arg0)
{
	if (p_arg0 == NULL) {
		return *this << "(null)";
	}
	VsULtoa((unsigned long) p_arg0, (char*) m_numberBuffer, 16);
	InternalFormatNum();
	return *this << "0x" << m_formattedText;
}

// FUNCTION: LEMBALL 0x004586d0
CVsOStream& CVsOStream::operator<<(unsigned char p_value)
{
	return *this << (char) p_value;
}

// FUNCTION: LEMBALL 0x004586e0
CVsOStream& CVsOStream::operator<<(unsigned long p_arg0)
{
	VsULtoa(p_arg0, (char*) m_numberBuffer, m_radix);
	InternalFormatNum();
	return *this << m_formattedText;
}

// FUNCTION: LEMBALL 0x00458720
CVsOStream& CVsOStream::operator<<(unsigned int p_arg0)
{
	VsULtoa(p_arg0, (char*) m_numberBuffer, m_radix);
	InternalFormatNum();
	return *this << m_formattedText;
}

// FUNCTION: LEMBALL 0x00458780
CVsOStream& CVsOStream::operator<<(Hex p_arg0)
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

// FUNCTION: LEMBALL 0x004589c0
CVsOStream& CVsOStream::operator<<(Hex8 p_arg0)
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

// FUNCTION: LEMBALL 0x00458d40
CVsOStream& CVsOStream::operator<<(CHAR4 p_arg0)
{
	unsigned long value = p_arg0.m_value;
	for (int i = 3; i >= 0; --i) {
		m_streamBuffer->Sputc((char) (value >> (char) (i * 8)));
	}
	return *this;
}

// FUNCTION: LEMBALL 0x0045bad0
CVsOStream& operator<<(CVsOStream& p_arg0, Rname p_arg1)
{
	p_arg0 << (int) p_arg1.m_value;
	return p_arg0;
}

// GLOBAL: LEMBALL 0x004a93a8
CVsOStream* g_pErrorOutput;

// GLOBAL: LEMBALL 0x004a97b8
CVsOStream* g_pSysOutput;

// GLOBAL: LEMBALL 0x004a97bc
CVsOStream* g_pDebugOutput;
