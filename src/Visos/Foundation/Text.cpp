#include "Text.h"

#include "../Graphics/Gdi.h"
#include "../Graphics/VsGdi.h"
#include "../Resources/ResFont.h"
#include "../Resources/ResZrle.h"
#include "VsDebugStreambuf.h"
#include "VsOStream.h"
#include "VsPoint.h"

// 68K 0x10117aa4 Set__5CTextFiiP8CResFONTPCcUlP6CRemap
// FUNCTION: LEMBALL 0x00469a50
void Text::Set(int p_x, int p_y, ResFont* p_font, char* p_text, unsigned long p_flags, Remap* p_remap)
{
	m_startX = (short) p_x;
	m_startY = (short) p_y;
	m_font = p_font;
	m_remap = p_remap;
	m_flags = p_flags;
	m_text = p_text;
}

// 68K 0x10116384 Set__5CTextFRC8CVSPointP8CResFONTPCcUlP6CRemap
// FUNCTION: LEMBALL 0x00469a80
void Text::Set(VsPoint& p_position, ResFont* p_font, char* p_text, unsigned long p_flags, Remap* p_remap)
{
	m_startX = p_position.m_x;
	m_startY = p_position.m_y;
	m_font = p_font;
	m_remap = p_remap;
	m_flags = p_flags;
	m_text = p_text;
}

// 68K 0x10117a40 Set__5CTextFiiP8CResFONT7CStringUlP6CRemap
// FUNCTION: LEMBALL 0x00469ac0
void Text::Set(int p_x, int p_y, ResFont* p_font, String p_text, unsigned long p_flags, Remap* p_remap)
{
	m_startX = (short) p_x;
	m_startY = (short) p_y;
	m_font = p_font;
	m_flags = p_flags;
	m_remap = p_remap;
	m_text = p_text.m_text;
}

// 68K 0x101163ea Set__5CTextFRC8CVSPointP8CResFONT7CStringUlP6CRemap
// FUNCTION: LEMBALL 0x00469b00
void Text::Set(VsPoint& p_position, ResFont* p_font, String p_text, unsigned long p_flags, Remap* p_remap)
{
	m_startX = p_position.m_x;
	m_startY = p_position.m_y;
	m_font = p_font;
	m_flags = p_flags;
	m_remap = p_remap;
	m_text = p_text.m_text;
}

// 68K 0x10101e00 Draw__5CTextFP4CGDI
// FUNCTION: LEMBALL 0x00469b40
void Text::Draw(Gdi* p_gdi)
{
	m_useAdvance = 0;
	p_gdi->AddToList(this);
}

// 68K 0x10101dac __dt__5CTextFv
Text::~Text()
{
}

// 68K 0x102119ee NextPos__5CTextFv
// FUNCTION: LEMBALL 0x004749c0
void Text::NextPos()
{
	short stepY;
	short stepX;
	if (m_useAdvance != 0) {
		stepX = m_advanceX;
		stepY = m_advanceY;
	}
	else {
		stepX = m_glyph->m_width + 1;
		stepY = m_glyph->m_height + 1;
	}
	unsigned int flags = m_flags;
	if ((flags & 0x200) != 0) {
		stepX = stepX + m_offsetX;
		stepY = stepY + m_offsetY;
	}
	if ((flags & 0x40) != 0) {
		m_x = m_x - stepX;
	}
	else if ((flags & 0x20) != 0) {
		m_x = m_x + stepX;
	}
	flags = m_flags;
	if ((flags & 0x80) != 0) {
		m_y = m_y - stepY;
		return;
	}
	if ((flags & 0x100) != 0) {
		m_y = m_y + stepY;
	}
}

// 68K 0x10211ad8 Render__5CTextFP4CGDI
// FUNCTION: LEMBALL 0x00474a20
void Text::Render(Gdi* p_gdi)
{
	ResFont* font = m_font;
	if (font->m_loaded != 0) {
		font->m_age = 0;
	}
	else {
		font->LoadData();
	}
	font->m_directUseCount++;
	m_x = m_startX;
	m_primitive.m_flags = m_flags;
	m_y = m_startY;
	m_primitive.m_remap = m_remap;
	const char* text = m_text;
	if (*text != '\0') {
		do {
			m_glyph = m_font->AsciItoZrle((unsigned char) *text);
			if (m_glyph == 0) {
				m_glyph = m_font->AsciItoZrle('I');
				if (m_glyph == 0) {
					m_glyph = m_font->m_animationEntries;
				}
				if (*text != ' ' || m_glyph == 0) {
					*g_pDebugOutput << "Letter '" << *text << "' not found in font " << Rname(m_font->m_resourceId)
									<< "\n";
				}
				NextPos();
			}
			else {
				if ((m_flags & 0xc0) != 0) {
					NextPos();
				}
				m_primitive.m_x = m_x;
				m_primitive.m_y = m_y;
				p_gdi->m_renderTarget->Blit(&m_primitive, m_glyph);
				if ((m_flags & 0xc0) == 0) {
					NextPos();
				}
			}
			text++;
		} while (*text != '\0');
	}
	m_font->m_directUseCount--;
}
