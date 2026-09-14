#ifndef LEMBALL_VISOS_FOUNDATION_VSSIZE_H
#define LEMBALL_VISOS_FOUNDATION_VSSIZE_H

#include "../../Common.h"

// SIZE 0x04
struct VsSize {
	VsSize() { m_width = m_height = 0; }
#ifdef LEMBALL_INLINE_VSSIZE_VALUE
	VsSize(short p_width, short p_height) : m_width(p_width), m_height(p_height) {}
#else
	VsSize(short p_width, short p_height);
#endif
#ifdef LEMBALL_INLINE_VSSIZE_COPY
	VsSize(const VsSize& p_source) : m_width(p_source.m_width), m_height(p_source.m_height) {}
#else
	VsSize(const VsSize& p_source);
#endif
	VsSize& operator=(const VsSize& p_source);

	short m_width;  // 0x00
	short m_height; // 0x02
};

#endif
