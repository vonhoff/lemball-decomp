#include "TargetTextLineBuffer.h"

#include "TargetTextLine.h"

// FUNCTION: LEMBALL 0x004738e0
void TargetTextLineBuffer::AddLine(const char* p_text, unsigned int p_color)
{
	if (m_count == m_capacity) {
		for (int line = 0; line < m_capacity - 1; line++) {
			m_lines[line].SetText(m_lines[line + 1].m_text, m_lines[line + 1].m_textColor);
		}
	}
	m_lines[m_count].SetText(p_text, p_color);
	if (m_capacity - m_count != 1) {
		m_count++;
	}
}

// FUNCTION: LEMBALL 0x00473a10
void TargetTextLineBuffer::AddText(char* p_text, unsigned int p_color)
{
	char* cursor = p_text;
	while (*cursor != 0) {
		if (*cursor == '\n') {
			*cursor = 0;
			AddLine(p_text, p_color);
			p_text = cursor + 1;
		}
		cursor++;
	}
	if (p_text < cursor) {
		AddLine(p_text, p_color);
	}
}
