#include "CText.h"

#include "Visos/Graphics/Primitives/CGDI.h"
#include "Visos/Graphics/Surfaces/CSurface.h"
#include "Visos/Resources/Types/CResFONT.h"
#include "Visos/Resources/Types/CResZRLE.h"
#include "Visos/Streams/CVSOStream.h"
#include "Visos/Math/CVSPoint.h"
#include "Visos/Strings/CString.h"
#include "Visos/Graphics/Primitives/CZRLE.h"

#include <stddef.h>

// FUNCTION: LEMBALL 0x00469a50
void CText::Set(int p_x, int p_y, CResFONT* p_font, char* p_text, unsigned long p_flags, CRemap* p_remap)
{
	m_startX = (short) p_x;
	m_startY = (short) p_y;
	m_font = p_font;
	m_remap = p_remap;
	m_flags = p_flags;
	m_text = p_text;
}

// FUNCTION: LEMBALL 0x00469a80
void CText::Set(CVSPoint& p_position, CResFONT* p_font, char* p_text, unsigned long p_flags, CRemap* p_remap)
{
	m_startX = p_position.m_x;
	m_startY = p_position.m_y;
	m_font = p_font;
	m_remap = p_remap;
	m_flags = p_flags;
	m_text = p_text;
}

// FUNCTION: LEMBALL 0x00469ac0
void CText::Set(int p_x, int p_y, CResFONT* p_font, CString p_text, unsigned long p_flags, CRemap* p_remap)
{
	m_startX = (short) p_x;
	m_startY = (short) p_y;
	m_font = p_font;
	m_flags = p_flags;
	m_remap = p_remap;
	m_text = p_text.m_text;
}

// FUNCTION: LEMBALL 0x00469b00
void CText::Set(CVSPoint& p_position, CResFONT* p_font, CString p_text, unsigned long p_flags, CRemap* p_remap)
{
	m_startX = p_position.m_x;
	m_startY = p_position.m_y;
	m_font = p_font;
	m_flags = p_flags;
	m_remap = p_remap;
	m_text = p_text.m_text;
}

// FUNCTION: LEMBALL 0x00469b40
void CText::Draw(CGDI* p_gdi)
{
	m_useAdvance = 0;
	p_gdi->AddToList(this);
}

// FUNCTION: LEMBALL 0x004749c0
void CText::NextPos()
{
	short stepY;
	short stepX;
	if (m_useAdvance != 0) {
		stepX = m_advanceX;
		stepY = m_advanceY;
	}
	else {
		stepY = m_glyph->m_height + 1;
		stepX = m_glyph->m_width + 1;
	}
	unsigned int flags = m_flags;
	if ((flags & TEXT_ADVANCE_USE_CUSTOM_OFFSETS) != 0) {
		stepX = stepX + m_offsetX;
		stepY = stepY + m_offsetY;
	}
	if ((flags & TEXT_ADVANCE_X_NEGATIVE) != 0) {
		m_x = m_x - stepX;
	}
	else if ((flags & TEXT_ADVANCE_X_POSITIVE) != 0) {
		m_x = m_x + stepX;
	}
	flags = m_flags;
	if ((flags & TEXT_ADVANCE_Y_NEGATIVE) != 0) {
		m_y = m_y - stepY;
		return;
	}
	if ((flags & TEXT_ADVANCE_Y_POSITIVE) != 0) {
		m_y = m_y + stepY;
	}
}

// FUNCTION: LEMBALL 0x00474a20
void CText::Render(CGDI* p_gdi)
{
	CResFONT* font = m_font;
	if (font->m_loaded != 0) {
		font->m_age = 0;
	}
	else {
		font->LoadData();
	}
	font->m_directUseCount++;
	m_x = m_startX;
	m_y = m_startY;
	m_primitive.m_flags = m_flags;
	m_primitive.m_remap = m_remap;
	const char* text = m_text;
	if (*text != '\0') {
		do {
			m_glyph = m_font->ASCIItoZRLE((unsigned char) *text);
			if (m_glyph == NULL) {
				m_glyph = m_font->ASCIItoZRLE('I');
				if (m_glyph == NULL) {
					m_glyph = m_font->m_animationEntries;
				}
				if (*text != ' ' || m_glyph == NULL) {
					*g_pDebugOutput << "Letter '" << *text << "' not found in font " << Rname(m_font->m_resourceId)
									<< "\n";
				}
				NextPos();
			}
			else {
				if ((m_flags & TEXT_ADVANCE_BEFORE_GLYPH_MASK) != 0) {
					NextPos();
				}
				m_primitive.m_x = m_x;
				m_primitive.m_y = m_y;
				p_gdi->m_renderTarget->Blit(&m_primitive, m_glyph);
				if ((m_flags & TEXT_ADVANCE_BEFORE_GLYPH_MASK) == 0) {
					NextPos();
				}
			}
			text++;
		} while (*text != '\0');
	}
	m_font->m_directUseCount--;
}
