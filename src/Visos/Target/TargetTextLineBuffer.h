#ifndef LEMBALL_VISOS_TARGET_TARGETTEXTLINEBUFFER_H
#define LEMBALL_VISOS_TARGET_TARGETTEXTLINEBUFFER_H

#include "../../Common.h"
#include "TargetTextLine.h"

// SIZE 0x0c
struct TargetTextLineBuffer {
	TargetTextLineBuffer(int p_capacity)
	{
		m_lines = new TargetTextLine[p_capacity];
		m_capacity = p_capacity;
		m_count = 0;
	}
	~TargetTextLineBuffer() { delete[] m_lines; }
	void AddLine(const char* p_text, unsigned int p_color);
	void AddText(char* p_text, unsigned int p_color);

	int m_capacity;          // 0x00
	TargetTextLine* m_lines; // 0x04
	int m_count;             // 0x08
};

#endif
