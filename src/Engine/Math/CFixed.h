#ifndef LEMBALL_VISOS_FOUNDATION_CFIXED_H
#define LEMBALL_VISOS_FOUNDATION_CFIXED_H

#include "Platform/Windows/Diagnostics/VsDebug.h"

// SIZE 0x04
class CFixed {
public:
	// FUNCTION: LEMBALL 0x0045a9a0
	CFixed() : m_value(DEBUG_SENTINEL) {}
	CFixed(int p_value) : m_value(p_value) {}
	CFixed(const CFixed& p_other) : m_value(p_other.m_value) {}

	int m_value; // 0x00
};

CFixed operator+(const CFixed& p_left, const CFixed& p_right);
#endif
