#ifndef LEMBALL_VISOS_FOUNDATION_VSSIZE_H
#define LEMBALL_VISOS_FOUNDATION_VSSIZE_H

// SIZE 0x04
struct VsSize {
	VsSize() { m_width = m_height = 0; }
	// FUNCTION: LEMBALL 0x0046daf0
	VsSize(short p_width, short p_height) : m_width(p_width), m_height(p_height) {}
	// FUNCTION: LEMBALL 0x0046db10
	VsSize(const VsSize& p_source) : m_width(p_source.m_width), m_height(p_source.m_height) {}
	VsSize& operator=(const VsSize& p_source);

	short m_width;  // 0x00
	short m_height; // 0x02
};

#endif
