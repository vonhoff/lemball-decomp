#ifndef LEMBALL_VISOS_TARGET_TARGETTEXTLINEBUFFER_H
#define LEMBALL_VISOS_TARGET_TARGETTEXTLINEBUFFER_H

#include "../../Common.h"

// SIZE 0x0c
struct TargetTextLineBuffer {
	void AddLine(const char* p_text, unsigned int p_color);
	void AddText(char* p_text, unsigned int p_color);

	int m_capacity;          // 0x00
	TargetTextLine* m_lines; // 0x04
	int m_count;             // 0x08
};

#endif
