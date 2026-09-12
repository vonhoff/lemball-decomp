#include "TargetTextLine.h"

// FUNCTION: LEMBALL 0x004564c0
TargetTextLine::~TargetTextLine()
{
	if (m_text != 0) {
		free(m_text);
	}
}

// FUNCTION: LEMBALL 0x004749b0
TargetTextLine::TargetTextLine()
{
	m_text = 0;
	m_textColor = 0;
	m_selected = 0;
}
