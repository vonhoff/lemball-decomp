#include "CText.h"

#include "Engine/Graphics/Primitives/CGDI.h"
#include "Engine/Graphics/Primitives/CZRLE.h"
#include "Engine/Math/CVSPoint.h"
#include "Engine/Math/CVSSize.h"
#include "Engine/Resources/Types/CResFONT.h"
#include "Engine/Resources/Types/CResZRLE.h"
#include "Engine/Streams/CVSOStream.h"
#include "Engine/Strings/CString.h"
#include "Engine/Text/TextAdvanceFlags.h"
#include "Platform/Windows/Graphics/CSurface.h"

#include <stddef.h>

// FUNCTION: LEMBALL 0x00469a50
void CText::Set(int p_x, int p_y, CResFONT* p_font, const char* p_text, unsigned long p_flags, CRemap* p_remap)
{
	m_startX = p_x;
	m_startY = p_y;
	m_font = p_font;
	m_remap = p_remap;
	m_flags = p_flags;
	m_text = p_text;
}

// FUNCTION: LEMBALL 0x00469a80
void CText::Set(const CVSPoint& p_position,
				CResFONT* p_font,
				const char* p_text,
				unsigned long p_flags,
				CRemap* p_remap)
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
	m_startX = p_x;
	m_startY = p_y;
	m_font = p_font;
	m_flags = p_flags;
	m_remap = p_remap;
	m_text = p_text.m_text;
}

// FUNCTION: LEMBALL 0x00469b00
void CText::Set(const CVSPoint& p_position, CResFONT* p_font, CString p_text, unsigned long p_flags, CRemap* p_remap)
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

inline static void AddAdvance(CVSSize& p_destination, const CVSSize& p_delta)
{
	p_destination.m_width += p_delta.m_width;
	p_destination.m_height += p_delta.m_height;
}

// FUNCTION: LEMBALL 0x004749c0
void CText::NextPos()
{
	CVSSize step;
	if (m_useAdvance != 0) {
		step = CVSSize(m_advanceX, m_advanceY);
	}
	else {
		step = m_glyph->m_size;
		AddAdvance(step, CVSSize(1, 1));
	}
	unsigned int flags = m_flags;
	if ((flags & TEXT_ADVANCE_USE_CUSTOM_OFFSETS) != 0) {
		AddAdvance(step, CVSSize(m_offsetX, m_offsetY));
	}
	if ((flags & TEXT_ADVANCE_X_NEGATIVE) != 0) {
		m_x = m_x - step.m_width;
	}
	else if ((flags & TEXT_ADVANCE_X_POSITIVE) != 0) {
		m_x = m_x + step.m_width;
	}
	flags = m_flags;
	if ((flags & TEXT_ADVANCE_Y_NEGATIVE) != 0) {
		m_y = m_y - step.m_height;
		return;
	}
	if ((flags & TEXT_ADVANCE_Y_POSITIVE) != 0) {
		m_y = m_y + step.m_height;
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
					*g_pDebugOutput << "Letter '" << *text << "' not found in font " << RNAME(m_font->m_resourceId)
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
