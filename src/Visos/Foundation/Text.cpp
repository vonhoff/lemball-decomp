#include "Text.h"

#include "../Graphics/Gdi.h"
#include "../Resources/ResZrle.h"
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
// SYNTHETIC: LEMBALL 0x00469b80
// Text::`scalar deleting destructor'
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
// STUB: LEMBALL 0x00474a20
void Text::Render(Gdi* p_gdi)
{
	// STRING: LEMBALL 0x004a2d34 "' not found in font "
	// STRING: LEMBALL 0x004a2d28 "Letter '"
}
