#ifndef LEMBALL_VISOS_GRAPHICS_CCLIPRECT_H
#define LEMBALL_VISOS_GRAPHICS_CCLIPRECT_H

#include "Engine/Math/CVSRect.h"
#include "CPrimitive.h"

// SIZE 0x10
// VTABLE: LEMBALL 0x00496cb8
class CClipRect : public CPrimitive {
public:
	enum ClipFlags {
		CLIP_REPLACE = 0,
		CLIP_EXPAND_BOUNDS = 0x1000,
		CLIP_IGNORE_PARENT = 0x10000
	};
	// FUNCTION: LEMBALL 0x00439520
	CClipRect() {}
	virtual void Draw(CGDI* p_gdi);   // vtable+0x04
	virtual void Render(CGDI* p_gdi); // vtable+0x08
	// FUNCTION: LEMBALL 0x00439730 FOLDED
	virtual ~CClipRect() {} // vtable+0x00

	friend class CGunController;
	friend class CAboutScreen;
	friend class CHiliteController;
	friend class CPasswordDrawer;
	friend class CSurface;
	friend class C2D;
	friend class CPVButton;

private:
	CVSRect m_bounds;     // 0x04
	unsigned int m_flags; // 0x0c
};

// SYNTHETIC: LEMBALL 0x00432a90
// CClipRect::`scalar deleting destructor'

#endif
