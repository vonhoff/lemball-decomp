#ifndef LEMBALL_VISOS_GRAPHICS_BITMAPRES_H
#define LEMBALL_VISOS_GRAPHICS_BITMAPRES_H

#include "BitmapResBase.h"

// SIZE 0x24
// VTABLE: LEMBALL 0x00497918
class BitmapRes : public BitmapResBase {
public:
	// FUNCTION: LEMBALL 0x00447120
	BitmapRes()
	{
		m_unknown22 = 0;
		m_unknown20 = 0;
	}
	virtual void Draw(CGdi* p_gdi);   // vtable+0x04
	virtual void Render(CGdi* p_gdi); // vtable+0x08
	// FUNCTION: LEMBALL 0x00447260
	virtual ~BitmapRes() {} // vtable+0x00

	friend class CBaseFrontendDrawer;
	friend class CMainOptions1Drawer;
	friend class CMainOptions2Drawer;
	friend class CPasswordDrawer;
	friend class CPreviewDrawer;
	friend class CSuccFailDrawer;
	friend class CSurface;
	friend class CCdLoadAnim;
	friend class TargetAboutScreen;

private:
	unsigned int m_unknown1c; // 0x1c
	short m_unknown20;        // 0x20
	short m_unknown22;        // 0x22
};

// SYNTHETIC: LEMBALL 0x00447170
// BitmapRes::`scalar deleting destructor'

#endif
