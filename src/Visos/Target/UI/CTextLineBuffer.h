#ifndef LEMBALL_VISOS_TARGET_UI_CTEXTLINEBUFFER_H
#define LEMBALL_VISOS_TARGET_UI_CTEXTLINEBUFFER_H

#include "CTextLine.h"

// SIZE 0x0c
struct CTextLineBuffer {
	CTextLineBuffer(int p_capacity)
	{
		m_lines = new CTextLine[p_capacity];
		m_capacity = p_capacity;
		m_count = 0;
	}
	~CTextLineBuffer() { delete[] m_lines; }
	void AddLine(const char* p_text, unsigned int p_color);
	void AddText(char* p_text, unsigned int p_color);

	int m_capacity;     // 0x00
	CTextLine* m_lines; // 0x04
	int m_count;        // 0x08
};

#endif
