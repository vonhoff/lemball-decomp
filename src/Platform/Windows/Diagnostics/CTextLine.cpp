#include "CTextLine.h"

#include <stddef.h>

// FUNCTION: LEMBALL 0x004564c0
CTextLine::~CTextLine()
{
	if (m_text != NULL) {
		free(m_text);
	}
}

// FUNCTION: LEMBALL 0x004749b0
CTextLine::CTextLine()
{
	m_text = NULL;
	m_textColour = 0;
	m_selected = 0;
}
