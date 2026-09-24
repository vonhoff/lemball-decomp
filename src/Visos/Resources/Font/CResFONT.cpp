#include "../CResFONT.h"

#include <string.h>

// FUNCTION: LEMBALL 0x0045db30
CVsSize* CResFONT::GetSize(CVsSize* p_result, const char* p_text, unsigned int p_flags)
{
	int textIndex = 0;
	CVsSize size;
	size.m_height = 0;
	size.m_width = 0;
	if (p_text[0] != '\0') {
		do {
			CResZRLE* glyph = ASCIItoZRLE(p_text[textIndex]);
			if (glyph == 0) {
				glyph = ASCIItoZRLE('I');
				if (glyph == 0) {
					glyph = m_animationEntries;
				}
			}
			short* glyphDimensions = &glyph->m_width;
			short* glyphOrigin = &glyph->m_x;
			if ((p_flags & 0x60) != 0) {
				size.m_width += *glyphDimensions + 1;
			}
			else {
				if (glyphOrigin[0] + *glyphDimensions > size.m_width) {
					size.m_width = glyphOrigin[0] + *glyphDimensions;
				}
			}
			if ((p_flags & 0x180) != 0) {
				size.m_height += glyphDimensions[1] + 1;
			}
			else {
				if (glyphOrigin[1] + glyphDimensions[1] > size.m_height) {
					size.m_height = glyphDimensions[1] + glyphOrigin[1];
				}
			}
			textIndex++;
		} while (p_text[textIndex] != '\0');
	}
	if ((p_flags & 0x60) != 0) {
		size.m_width--;
	}
	if ((p_flags & 0x180) != 0) {
		size.m_height--;
	}
	memcpy(&p_result->m_width, &size.m_width, sizeof(size.m_width));
	memcpy(&p_result->m_height, &size.m_height, sizeof(size.m_height));
	return p_result;
}
