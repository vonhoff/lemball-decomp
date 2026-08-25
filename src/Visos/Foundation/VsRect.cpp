#include "VsRect.h"

// 68K 0x1011c926 __ct__7CVSRectFv
// FUNCTION: LEMBALL 0x00442170
VsRect::VsRect()
{
	m_height = 0;
	m_width = 0;
	m_y = 0;
	m_x = 0;
}

// FUNCTION: LEMBALL 0x00447270
VsRect::VsRect(short p_x, short p_y, short p_width, short p_height)
{
	m_width = p_width;
	m_height = p_height;
	m_x = p_x;
	m_y = p_y;
}
