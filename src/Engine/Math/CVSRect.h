#ifndef LEMBALL_VISOS_FOUNDATION_CVSRECT_H
#define LEMBALL_VISOS_FOUNDATION_CVSRECT_H

#include "CVSPoint.h"
#include "CVSSize.h"

// SIZE 0x08
class CVSRect : public CVSSize, public CVSPoint {
public:
	CVSRect() {}

	CVSRect(short p_x, short p_y, short p_width, short p_height) : CVSSize(p_width, p_height), CVSPoint(p_x, p_y) {}
	CVSRect(const CVSPoint& p_position, const CVSSize& p_size) : CVSSize(p_size), CVSPoint(p_position) {}
	CVSRect(short p_x, short p_y, const CVSSize& p_size);
	CVSRect(const CVSRect& p_source);

	friend class CGDI;
	friend class CGWnd;
	friend class CPVWnd;
	friend class CWnd;
	friend class CMain2DDisplay;
	friend class CIntroAnimDrawer;
	friend class CBaseFrontendDrawer;
	friend class CSurface;
	friend class CBaseCursor;

	CVSRect& operator=(const CVSRect& p_source);
	CVSRect* ExpandToInclude(const CVSRect& p_rect);
	void ClipToSize(const CVSSize& p_size)
	{
		if (m_x < 0) {
			m_width += m_x;
			m_x = 0;
		}
		if ((short) (m_width + m_x) > p_size.m_width) {
			m_width = p_size.m_width - m_x;
		}
		if (m_y < 0) {
			m_height += m_y;
			m_y = 0;
		}
		if ((short) (m_y + m_height) > p_size.m_height) {
			m_height = p_size.m_height - m_y;
		}
		if (m_width <= 0 || m_height <= 0) {
			m_width = m_height = m_x = m_y = 0;
		}
	}
};

// SYNTHETIC: LEMBALL 0x00442170
// CVSRect::CVSRect

// FUNCTION: LEMBALL 0x00447270
// ??0CVSRect@@QAE@FFFF@Z

// FUNCTION: LEMBALL 0x0044e6c0
inline CVSRect::CVSRect(const CVSRect& p_source) : CVSSize(p_source), CVSPoint(p_source)
{
}

// FUNCTION: LEMBALL 0x00478b80
inline CVSRect::CVSRect(short p_x, short p_y, const CVSSize& p_size) : CVSSize(p_size), CVSPoint(p_x, p_y)
{
}

#endif
