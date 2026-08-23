#ifndef LEMBALL_VISOS_TARGET_TARGETTEXTLINE_H
#define LEMBALL_VISOS_TARGET_TARGETTEXTLINE_H

#include "../../Common.h"

// SIZE 0x0c
struct TargetTextLine {
	unsigned int m_textColor; // 0x00
	char* m_text;             // 0x04
	int m_selected;           // 0x08
};

#endif
