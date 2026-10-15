#include "CCopyText.h"

#include <string.h>

#include "Engine/Math/CVSPoint.h"
#include "Engine/Strings/CString.h"

class CResFONT;
class CRemap;

// FUNCTION: LEMBALL 0x0046a080
void CCopyText::Set(int p_x, int p_y, CResFONT* p_font, const char* p_text, unsigned long p_flags, CRemap* p_remap)
{
	strcpy(m_buffer, p_text);
	m_startX = p_x;
	m_startY = p_y;
	m_font = p_font;
	m_text = m_buffer;
	m_remap = p_remap;
	m_flags = p_flags;
}

// FUNCTION: LEMBALL 0x0046a0e0
void CCopyText::Set(const CVSPoint& p_position,
					CResFONT* p_font,
					const char* p_text,
					unsigned long p_flags,
					CRemap* p_remap)
{
	strcpy(m_buffer, p_text);
	m_startX = p_position.m_x;
	m_startY = p_position.m_y;
	m_font = p_font;
	m_remap = p_remap;
	m_flags = p_flags;
	m_text = m_buffer;
}

// FUNCTION: LEMBALL 0x0046a140
void CCopyText::Set(int p_x, int p_y, CResFONT* p_font, CString p_text, unsigned long p_flags, CRemap* p_remap)
{
	strcpy(m_buffer, p_text.m_text);
	m_startX = p_x;
	m_startY = p_y;
	m_font = p_font;
	m_text = m_buffer;
	m_remap = p_remap;
	m_flags = p_flags;
}

// FUNCTION: LEMBALL 0x0046a1b0
void CCopyText::Set(const CVSPoint& p_position,
					CResFONT* p_font,
					CString p_text,
					unsigned long p_flags,
					CRemap* p_remap)
{
	strcpy(m_buffer, p_text.m_text);
	m_startX = p_position.m_x;
	m_startY = p_position.m_y;
	m_font = p_font;
	m_remap = p_remap;
	m_flags = p_flags;
	m_text = m_buffer;
}
