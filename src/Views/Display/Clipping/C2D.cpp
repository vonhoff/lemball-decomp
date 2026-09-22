#include "../C2D.h"

// FUNCTION: LEMBALL 0x004368f0
CVsRect* C2D::GetClipRectangle()
{
	static CVsRect clipRectangle;
	CVsRect rectangle((short) m_clipOffsetX, (short) m_clipOffsetY, m_clipSize.m_x, m_clipSize.m_y);
	clipRectangle = rectangle;
	return &clipRectangle;
}
