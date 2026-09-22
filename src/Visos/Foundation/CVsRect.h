#ifndef LEMBALL_VISOS_FOUNDATION_CVSRECT_H
#define LEMBALL_VISOS_FOUNDATION_CVSRECT_H

#include "CVsPoint.h" // complete type
#include "CVsSize.h"  // complete type

// SIZE 0x08
class CVsRect : public CVsSize, public CVsPoint {
public:
	CVsRect() {}

	CVsRect(short p_x, short p_y, short p_width, short p_height) : CVsSize(p_width, p_height), CVsPoint(p_x, p_y) {}
	CVsRect(short p_x, short p_y, CVsSize* p_size);
	CVsRect(const CVsRect& p_source);

	friend class CGDI;
	friend class CGWnd;
	friend class CPVWnd;
	friend class CWnd;
	friend class CMain2DDisplay;
	friend class CIntroAnimDrawer;
	friend class CBaseFrontendDrawer;
	friend class CSurface;
	friend class CBaseCursor;

	CVsRect& operator=(const CVsRect& p_source);
	void ExpandToInclude(const CVsRect& p_rect);
};

// SYNTHETIC: LEMBALL 0x00442170
// CVsRect::CVsRect

// FUNCTION: LEMBALL 0x00447270
// ??0CVsRect@@QAE@FFFF@Z

// FUNCTION: LEMBALL 0x0044e6c0
inline CVsRect::CVsRect(const CVsRect& p_source) : CVsSize(p_source), CVsPoint(p_source)
{
}

#endif
