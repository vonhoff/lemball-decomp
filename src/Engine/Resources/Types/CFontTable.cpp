#include "Engine/Resources/Types/CFontTable.h"

#include "Engine/Resources/Types/CResFONT.h"
#include "Engine/Resources/Types/CResBaseLIST.h"
#include "Engine/Resources/Types/CResINT.h"

#include <stddef.h>

#define FONT_GLYPH_TABLE_BYTES 0x400
#define FONT_GLYPH_COUNT 0x100

// FUNCTION: LEMBALL 0x00473650
CFontTable::CFontTable(CResFONT* p_font)
{
	unsigned int offset;
	unsigned int index;
	int glyphIndex;

	m_glyphs = (CResZRLE**) ::operator new(FONT_GLYPH_TABLE_BYTES);
	offset = 0;
	do {
		m_glyphs[offset] = NULL;
		offset++;
	} while (offset < FONT_GLYPH_COUNT);

	index = 0;
	if (p_font->m_totalSize / p_font->m_listHeader->m_headerSize != 0) {
		do {
			if (p_font->m_fontEntries == NULL) {
				glyphIndex = p_font->m_fontTable->GetChar(&p_font->m_animationEntries[index]);
			}
			else {
				glyphIndex = p_font->m_fontEntries[index].m_value;
			}
			m_glyphs[glyphIndex] = &p_font->m_animationEntries[index];
			index++;
		} while (index < p_font->m_totalSize / p_font->m_listHeader->m_headerSize);
	}
}

// FUNCTION: LEMBALL 0x00473700
CResZRLE* CFontTable::GetZRLE(int p_character)
{
	return m_glyphs[p_character];
}

// FUNCTION: LEMBALL 0x00473710
char CFontTable::GetChar(CResZRLE* p_glyph)
{
	int i = 0;
	CResZRLE** glyphs = m_glyphs;
	do {
		if (*glyphs == p_glyph) {
			return (char) i;
		}
		glyphs++;
		i++;
	} while (i < FONT_GLYPH_COUNT);
	return FONT_CHARACTER_NOT_FOUND;
}

// FUNCTION: LEMBALL 0x00473730
CFontTable::~CFontTable()
{
	::operator delete(m_glyphs);
}
