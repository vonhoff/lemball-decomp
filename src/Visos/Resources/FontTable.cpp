#include "FontTable.h"

#include "ResFont.h"

#define kGlyphTableBytes 0x400
#define kGlyphTableCount 0x100
#define kResIntSize 0x4c
#define kResZrleSize 0x54

// 68K 0x10210b68 __ct__10CFontTableFP8CResFONT
// FUNCTION: LEMBALL 0x00473650
FontTable::FontTable(ResFont* p_font)
{
	unsigned int offset;
	unsigned int index;
	int glyphIndex;
	int zrleOffset;

	m_glyphs = (ResZrle**) ::operator new(kGlyphTableBytes);
	offset = 0;
	do {
		offset += sizeof(ResZrle*);
		*(ResZrle**) ((unsigned char*) m_glyphs + offset - sizeof(ResZrle*)) = 0;
	} while (offset < kGlyphTableBytes);

	zrleOffset = 0;
	index = zrleOffset;
	if (p_font->m_totalSize / p_font->m_listHeader->m_headerSize != 0) {
		int intOffset = 0;
		do {
			if (p_font->m_fontEntries == 0) {
				glyphIndex = p_font->m_fontTable->GetChar(
					(ResZrle*) ((unsigned char*) p_font->m_animationEntries + zrleOffset));
			}
			else {
				glyphIndex = ((ResInt*) ((unsigned char*) p_font->m_fontEntries + intOffset))->m_value;
			}
			m_glyphs[glyphIndex] = (ResZrle*) ((unsigned char*) p_font->m_animationEntries + zrleOffset);
			intOffset += kResIntSize;
			zrleOffset += kResZrleSize;
			index++;
		} while (index < p_font->m_totalSize / p_font->m_listHeader->m_headerSize);
	}
}

// 68K 0x10210c14 GetZRLE__10CFontTableCFi
// FUNCTION: LEMBALL 0x00473700
ResZrle* FontTable::GetZrle(int p_character)
{
	return m_glyphs[p_character];
}

// 68K 0x10210c48 GetChar__10CFontTableCFP8CResZRLE
// FUNCTION: LEMBALL 0x00473710
char FontTable::GetChar(ResZrle* p_glyph)
{
	int i = 0;
	ResZrle** glyphs = m_glyphs;
	do {
		if (*glyphs == p_glyph) {
			return (char) i;
		}
		glyphs++;
		i++;
	} while (i < kGlyphTableCount);
	return -1;
}

// 68K 0x10210c9e __dt__10CFontTableFv
// SYNTHETIC: LEMBALL 0x00473750
// FontTable::`scalar deleting destructor'
FontTable::~FontTable()
{
}
