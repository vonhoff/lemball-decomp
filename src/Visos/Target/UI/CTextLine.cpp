#include "CTextLine.h"

// FUNCTION: LEMBALL 0x004564c0
CTextLine::~CTextLine()
{
	if (m_text != 0) {
		free(m_text);
	}
}

// FUNCTION: LEMBALL 0x004749b0
CTextLine::CTextLine()
{
	m_text = 0;
	m_textColor = 0;
	m_selected = 0;
}
