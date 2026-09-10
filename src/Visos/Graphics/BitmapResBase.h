#ifndef LEMBALL_VISOS_GRAPHICS_BITMAPRESBASE_H
#define LEMBALL_VISOS_GRAPHICS_BITMAPRESBASE_H

#include "../../Common.h"
#include "Primitive.h" // complete type

// SIZE 0x1c
// VTABLE: LEMBALL 0x00497928
class BitmapResBase : public Primitive {
public:
	// FUNCTION: LEMBALL 0x0044b5f0
	BitmapResBase() : m_x(m_y = 0)
	{
		m_height = 0;
		m_width = 0;
		m_sourceY = 0;
		m_sourceX = 0;
	}
	virtual void Draw(Gdi* p_gdi);   // vtable+0x04
	virtual void Render(Gdi* p_gdi); // vtable+0x08
	virtual ~BitmapResBase() {}      // vtable+0x00

	friend class BaseFrontendDrawer;
	friend class MainOptions1Drawer;
	friend class MainOptions2Drawer;
	friend class PasswordDrawer;
	friend class PreviewDrawer;
	friend class SuccFailDrawer;
	friend class Surface;
	friend class CdLoadAnimDraw;
	friend class TargetAboutScreen;

protected:
	short m_x;             // 0x04
	short m_y;             // 0x06
	short m_width;         // 0x08
	short m_height;        // 0x0a
	short m_sourceX;       // 0x0c
	short m_sourceY;       // 0x0e
	ResBitmap* m_resource; // 0x10
	unsigned int m_flags;  // 0x14
	Remap* m_remap;        // 0x18
};

// SYNTHETIC: LEMBALL 0x004471a0
// BitmapResBase::`scalar deleting destructor'

#endif
