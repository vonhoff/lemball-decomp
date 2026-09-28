#include "CFontTable.h"

#include "CResFONT.h"
#include "Visos/Resources/CResBaseLIST.h"
#include "Visos/Resources/CResINT.h"

#define FONT_GLYPH_TABLE_BYTES 0x400
#define FONT_GLYPH_COUNT 0x100
#define FONT_INT_RESOURCE_STRIDE 0x4c
#define FONT_ZRLE_RESOURCE_STRIDE 0x54

// FUNCTION: LEMBALL 0x00473650
CFontTable::CFontTable(CResFONT* p_font)
{
	unsigned int offset;
	unsigned int index;
	int glyphIndex;
	int zrleOffset;
	int intOffset;

	m_glyphs = (CResZRLE**) ::operator new(FONT_GLYPH_TABLE_BYTES);
	offset = 0;
	do {
		m_glyphs[offset] = 0;
		offset++;
	} while (offset < FONT_GLYPH_COUNT);

	zrleOffset = 0;
	index = zrleOffset;
	if (p_font->m_totalSize / p_font->m_listHeader->m_headerSize != 0) {
		intOffset = 0;
		do {
			if (p_font->m_fontEntries == 0) {
				glyphIndex = p_font->m_fontTable->GetChar(
					(CResZRLE*) ((unsigned char*) p_font->m_animationEntries + zrleOffset));
			}
			else {
				glyphIndex = ((CResINT*) ((unsigned char*) p_font->m_fontEntries + intOffset))->m_value;
			}
			m_glyphs[glyphIndex] = (CResZRLE*) ((unsigned char*) p_font->m_animationEntries + zrleOffset);
			intOffset += FONT_INT_RESOURCE_STRIDE;
			zrleOffset += FONT_ZRLE_RESOURCE_STRIDE;
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
	return -1;
}

// FUNCTION: LEMBALL 0x00473730
CFontTable::~CFontTable()
{
	::operator delete(m_glyphs);
}
