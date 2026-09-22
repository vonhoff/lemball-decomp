#ifndef LEMBALL_VISOS_GRAPHICS_BITMAPRESBASE_H
#define LEMBALL_VISOS_GRAPHICS_BITMAPRESBASE_H

#include "CPrimitive.h" // complete type

class CResBitmap;
class CRemap;

// SIZE 0x1c
// VTABLE: LEMBALL 0x00497928
class BitmapResBase : public CPrimitive {
public:
	// FUNCTION: LEMBALL 0x0044b5f0
	BitmapResBase() : m_x(m_y = 0)
	{
		m_height = 0;
		m_width = 0;
		m_sourceY = 0;
		m_sourceX = 0;
	}
	virtual void Draw(CGdi* p_gdi);   // vtable+0x04
	virtual void Render(CGdi* p_gdi); // vtable+0x08
	// FUNCTION: LEMBALL 0x0044b630
	virtual ~BitmapResBase() {} // vtable+0x00

	friend class CBaseFrontendDrawer;
	friend class CMainOptions1Drawer;
	friend class CMainOptions2Drawer;
	friend class CPasswordDrawer;
	friend class CPreviewDrawer;
	friend class CSuccFailDrawer;
	friend class CSurface;
	friend class CCdLoadAnim;
	friend class TargetAboutScreen;

protected:
	short m_x;              // 0x04
	short m_y;              // 0x06
	short m_width;          // 0x08
	short m_height;         // 0x0a
	short m_sourceX;        // 0x0c
	short m_sourceY;        // 0x0e
	CResBitmap* m_resource; // 0x10
	unsigned int m_flags;   // 0x14
	CRemap* m_remap;        // 0x18
};

// SYNTHETIC: LEMBALL 0x004471a0
// BitmapResBase::`scalar deleting destructor'

#endif
