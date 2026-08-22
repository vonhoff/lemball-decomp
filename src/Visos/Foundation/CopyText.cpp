#include "CopyText.h"

#include "VsPoint.h"

#include <string.h>

// 68K 0x10101bcc Set__9CCopyTextFiiP8CResFONTPCcUlP6CRemap
// FUNCTION: LEMBALL 0x0046a080
void CopyText::Set(int p_x, int p_y, ResFont* p_font, const char* p_text, unsigned long p_flags, Remap* p_remap)
{
	strcpy(m_buffer, p_text);
	m_text = m_buffer;
	m_startX = (short) p_x;
	m_startY = (short) p_y;
	m_font = p_font;
	m_remap = p_remap;
	m_flags = p_flags;
}

// 68K 0x10101d74 Set__9CCopyTextFRC8CVSPointP8CResFONTPCcUlP6CRemap
// FUNCTION: LEMBALL 0x0046a0e0
void CopyText::Set(const VsPoint& p_position,
				   ResFont* p_font,
				   const char* p_text,
				   unsigned long p_flags,
				   Remap* p_remap)
{
	strcpy(m_buffer, p_text);
	m_startX = p_position.m_x;
	m_startY = p_position.m_y;
	m_font = p_font;
	m_remap = p_remap;
	m_flags = p_flags;
	m_text = m_buffer;
}

// 68K 0x10101ce8 Set__9CCopyTextFiiP8CResFONT7CStringUlP6CRemap
// FUNCTION: LEMBALL 0x0046a140
void CopyText::Set(int p_x, int p_y, ResFont* p_font, String p_text, unsigned long p_flags, Remap* p_remap)
{
	strcpy(m_buffer, p_text.m_text);
	m_startX = (short) p_x;
	m_startY = (short) p_y;
	m_font = p_font;
	m_text = m_buffer;
	m_remap = p_remap;
	m_flags = p_flags;
}

// 68K 0x10101c52 Set__9CCopyTextFRC8CVSPointP8CResFONT7CStringUlP6CRemap
// FUNCTION: LEMBALL 0x0046a1b0
void CopyText::Set(const VsPoint& p_position, ResFont* p_font, String p_text, unsigned long p_flags, Remap* p_remap)
{
	strcpy(m_buffer, p_text.m_text);
	m_startX = p_position.m_x;
	m_startY = p_position.m_y;
	m_font = p_font;
	m_remap = p_remap;
	m_flags = p_flags;
	m_text = m_buffer;
}

// 68K 0x10101b46 __dt__9CCopyTextFv
// SYNTHETIC: LEMBALL 0x0046a220
// CopyText::`scalar deleting destructor'
CopyText::~CopyText()
{
	delete[] m_buffer;
}
