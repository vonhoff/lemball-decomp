#include "CVSOStream.h"

#include "CVSDebugStreambuf.h"
#include "Visos/Foundation/CVSIOs.h"
#include "Visos/Foundation/CVSStreambuf.h"
#include "VsString.h"

#include <string.h>

#pragma intrinsic(memset, memcpy, strlen)

// FUNCTION: LEMBALL 0x00458450
CVSOStream::CVSOStream(CVSStreambuf* p_streamBuffer) : CVSIOs(p_streamBuffer)
{
}

// FUNCTION: LEMBALL 0x004584a0
CVSOStream::~CVSOStream()
{
}

// FUNCTION: LEMBALL 0x004584c0
void CVSOStream::_FormatNum()
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
CVSOStream& CVSOStream::operator<<(const char* p_text)
{
	((CVSDebugStreambuf*) m_streamBuffer)->sputs((char*) p_text);
	return *this;
}

// FUNCTION: LEMBALL 0x004585d0
CVSOStream& CVSOStream::operator<<(char p_character)
{
	((CVSDebugStreambuf*) m_streamBuffer)->sputc(p_character);
	return *this;
}

// FUNCTION: LEMBALL 0x004585f0
CVSOStream& CVSOStream::operator<<(long p_value)
{
	vsLtoa(p_value, (char*) m_numberBuffer, m_radix);
	_FormatNum();
	return *this << m_formattedText;
}

// FUNCTION: LEMBALL 0x00458630
CVSOStream& CVSOStream::operator<<(int p_value)
{
	vsLtoa(p_value, (char*) m_numberBuffer, m_radix);
	_FormatNum();
	return *this << m_formattedText;
}

// FUNCTION: LEMBALL 0x00458670
CVSOStream& CVSOStream::operator<<(const void* p_pointer)
{
	if (p_pointer == NULL) {
		return *this << "(null)";
	}
	vsULtoa((unsigned long) p_pointer, (char*) m_numberBuffer, 16);
	_FormatNum();
	return *this << "0x" << m_formattedText;
}

// FUNCTION: LEMBALL 0x004586d0
CVSOStream& CVSOStream::operator<<(unsigned char p_value)
{
	return *this << (char) p_value;
}

// FUNCTION: LEMBALL 0x004586e0
CVSOStream& CVSOStream::operator<<(unsigned long p_value)
{
	vsULtoa(p_value, (char*) m_numberBuffer, m_radix);
	_FormatNum();
	return *this << m_formattedText;
}

// FUNCTION: LEMBALL 0x00458720
CVSOStream& CVSOStream::operator<<(unsigned int p_value)
{
	vsULtoa(p_value, (char*) m_numberBuffer, m_radix);
	_FormatNum();
	return *this << m_formattedText;
}

// FUNCTION: LEMBALL 0x00458780
CVSOStream& CVSOStream::operator<<(Hex p_value)
{
	unsigned int oldFlags = m_flags;
	m_flags = (oldFlags & ~0x8030) | 0x40;
	unsigned int oldRadix = m_radix;
	m_radix = 16;
	*this << (unsigned long) p_value.m_value;

	m_radix = oldRadix;
	m_flags = oldFlags;
	return *this;
}

// FUNCTION: LEMBALL 0x004589c0
CVSOStream& CVSOStream::operator<<(Hex8 p_value)
{
	char oldFill = m_fill;
	m_fill = '0';
	unsigned int oldWidth = m_width;
	m_width = 8;
	unsigned int oldFlags = m_flags;
	m_flags = (oldFlags & ~0x8030) | 0x40;
	unsigned int oldRadix = m_radix;
	m_radix = 16;
	*this << (unsigned long) p_value.m_value;

	m_width = oldWidth;
	m_radix = oldRadix;
	m_fill = oldFill;
	m_flags = oldFlags;
	return *this;
}

// FUNCTION: LEMBALL 0x00458d40
CVSOStream& CVSOStream::operator<<(CHAR4 p_value)
{
	unsigned long value = p_value.m_value;
	for (int i = 3; i >= 0; --i) {
		m_streamBuffer->sputc((char) (value >> (char) (i * 8)));
	}
	return *this;
}

// FUNCTION: LEMBALL 0x0045bad0
CVSOStream& operator<<(CVSOStream& p_stream, Rname p_resourceName)
{
	p_stream << (int) p_resourceName.m_value;
	return p_stream;
}

// GLOBAL: LEMBALL 0x004a93a8
CVSOStream* g_pErrorOutput;

// GLOBAL: LEMBALL 0x004a97b8
CVSOStream* g_pSysOutput;

// GLOBAL: LEMBALL 0x004a97bc
CVSOStream* g_pDebugOutput;
