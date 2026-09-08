#ifndef LEMBALL_VISOS_FOUNDATION_VSRECT_H
#define LEMBALL_VISOS_FOUNDATION_VSRECT_H

#include "../../Common.h"
#include "VsSize.h" // complete type

// SIZE 0x08
class VsRect : public VsSize {
public:
	VsRect()
	{
		m_height = 0;
		m_width = 0;
		m_y = 0;
		m_x = 0;
	}

	VsRect(short p_x, short p_y, short p_width, short p_height)
	{
		m_width = p_width;
		m_height = p_height;
		m_x = p_x;
		m_y = p_y;
	}
	VsRect(short p_x, short p_y, VsSize* p_size);
	VsRect(const VsRect& p_source);

	friend class Gdi;
	friend class GWnd;
	friend class PvWnd;
	friend class Wnd;
	friend class Main2DDisplay;
	friend class IntroAnimDrawer;
	friend class BaseFrontendDrawer;
	friend class Surface;
	friend class BaseCursor;

	VsRect& operator=(const VsRect& p_source);
	void ExpandToInclude(const VsRect& p_rect);

	short m_x; // 0x04
	short m_y; // 0x06
};

// 68K 0x1011c926 __ct__7CVSRectFv
// SYNTHETIC: LEMBALL 0x00442170
// VsRect::VsRect

// SYNTHETIC: LEMBALL 0x00447270
// VsRect::VsRect

#endif
