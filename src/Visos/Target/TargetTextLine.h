#ifndef LEMBALL_VISOS_TARGET_TARGETTEXTLINE_H
#define LEMBALL_VISOS_TARGET_TARGETTEXTLINE_H

#include "../../Common.h"

#include <stdlib.h>
#include <string.h>

// SIZE 0x0c
struct TargetTextLine {
	TargetTextLine();
	~TargetTextLine();
	void SetText(const char* p_text, unsigned int p_color)
	{
		if (m_text != 0) {
			free(m_text);
		}
		m_text = (char*) malloc(strlen(p_text) + 1);
		strcpy(m_text, p_text);
		m_textColor = p_color;
		m_selected = 0;
	}

	unsigned int m_textColor; // 0x00
	char* m_text;             // 0x04
	int m_selected;           // 0x08
};

#endif
