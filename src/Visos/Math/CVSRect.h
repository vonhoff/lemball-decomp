#ifndef LEMBALL_VISOS_FOUNDATION_CVSRECT_H
#define LEMBALL_VISOS_FOUNDATION_CVSRECT_H

#include "CVSPoint.h"
#include "CVSSize.h"

// SIZE 0x08
class CVSRect : public CVSSize, public CVSPoint {
public:
	CVSRect() {}

	CVSRect(short p_x, short p_y, short p_width, short p_height) : CVSSize(p_width, p_height), CVSPoint(p_x, p_y) {}
	CVSRect(short p_x, short p_y, CVSSize* p_size);
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
};

// SYNTHETIC: LEMBALL 0x00442170
// CVSRect::CVSRect

// FUNCTION: LEMBALL 0x00447270
// ??0CVSRect@@QAE@FFFF@Z

// FUNCTION: LEMBALL 0x0044e6c0
inline CVSRect::CVSRect(const CVSRect& p_source) : CVSSize(p_source), CVSPoint(p_source)
{
}

#endif
