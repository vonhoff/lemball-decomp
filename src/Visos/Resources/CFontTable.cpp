#include "CFontTable.h"

#include "CResFont.h"
#include "Visos/Resources/CResBaseList.h"
#include "Visos/Resources/CResInt.h"

#define kGlyphTableBytes 0x400
#define kGlyphTableCount 0x100
#define kResIntSize 0x4c
#define kResZrleSize 0x54

// FUNCTION: LEMBALL 0x00473650
CFontTable::CFontTable(CResFont* p_font)
{
	unsigned int offset;
	unsigned int index;
	int glyphIndex;
	int zrleOffset;
	int intOffset;

	m_glyphs = (CResZrle**) ::operator new(kGlyphTableBytes);
	offset = 0;
	do {
		m_glyphs[offset] = 0;
		offset++;
	} while (offset < kGlyphTableCount);

	zrleOffset = 0;
	index = zrleOffset;
	if (p_font->m_totalSize / p_font->m_listHeader->m_headerSize != 0) {
		intOffset = 0;
		do {
			if (p_font->m_fontEntries == 0) {
				glyphIndex = p_font->m_fontTable->GetChar(
					(CResZrle*) ((unsigned char*) p_font->m_animationEntries + zrleOffset));
			}
			else {
				glyphIndex = ((CResInt*) ((unsigned char*) p_font->m_fontEntries + intOffset))->m_value;
			}
			m_glyphs[glyphIndex] = (CResZrle*) ((unsigned char*) p_font->m_animationEntries + zrleOffset);
			intOffset += kResIntSize;
			zrleOffset += kResZrleSize;
			index++;
		} while (index < p_font->m_totalSize / p_font->m_listHeader->m_headerSize);
	}
}

// FUNCTION: LEMBALL 0x00473700
CResZrle* CFontTable::GetZrle(int p_character)
{
	return m_glyphs[p_character];
}

// FUNCTION: LEMBALL 0x00473710
char CFontTable::GetChar(CResZrle* p_glyph)
{
	int i = 0;
	CResZrle** glyphs = m_glyphs;
	do {
		if (*glyphs == p_glyph) {
			return (char) i;
		}
		glyphs++;
		i++;
	} while (i < kGlyphTableCount);
	return -1;
}

// FUNCTION: LEMBALL 0x00473730
CFontTable::~CFontTable()
{
	::operator delete(m_glyphs);
}
