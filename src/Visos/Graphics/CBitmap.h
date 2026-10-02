#ifndef LEMBALL_VISOS_GRAPHICS_CBITMAP_H
#define LEMBALL_VISOS_GRAPHICS_CBITMAP_H

#include "../Foundation/CVSRect.h"
#include "CPrimitive.h"

class CResBITMAP;
class CRemap;

// SIZE 0x1c
// VTABLE: LEMBALL 0x00497928
class CBitmap : public CPrimitive, public CVSPoint {
public:
	enum BitmapFlags {
		BITMAP_REVERSE_ROWS = 0x2,
		BITMAP_TRANSPARENT_ZERO = 0x800
	};
	// FUNCTION: LEMBALL 0x0044b5f0
	CBitmap() : m_sourceRect() {}
	virtual void Draw(CGDI* p_gdi);   // vtable+0x04
	virtual void Render(CGDI* p_gdi); // vtable+0x08
	// FUNCTION: LEMBALL 0x0044b630
	virtual ~CBitmap() {} // vtable+0x00

	friend class CBaseFrontendDrawer;
	friend class CMainOptions1Drawer;
	friend class CMainOptions2Drawer;
	friend class CPasswordDrawer;
	friend class CPreviewDrawer;
	friend class CSuccFailDrawer;
	friend class CSurface;
	friend class CCDLoadAnim;
	friend class CAboutScreen;

protected:
	CVSRect m_sourceRect;   // 0x08
	CResBITMAP* m_resource; // 0x10
	unsigned int m_flags;   // 0x14
	CRemap* m_remap;        // 0x18
};

// SYNTHETIC: LEMBALL 0x004471a0
// CBitmap::`scalar deleting destructor'

#endif
