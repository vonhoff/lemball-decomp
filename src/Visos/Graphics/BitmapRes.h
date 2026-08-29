#ifndef LEMBALL_VISOS_GRAPHICS_BITMAPRES_H
#define LEMBALL_VISOS_GRAPHICS_BITMAPRES_H

#include "../../Common.h"
#include "Primitive.h" // complete type

// SIZE 0x1c
// VTABLE: LEMBALL 0x00497928
class BitmapResBase : public Primitive {
public:
	BitmapResBase();
	virtual void Draw(Gdi* p_gdi);   // vtable+0x04
	virtual void Render(Gdi* p_gdi); // vtable+0x08
	virtual ~BitmapResBase() {}      // vtable+0x00

	friend class CdLoadAnimDraw;
	friend class Surface;

private:
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

// SYNTHETIC: LEMBALL 0x00403166
// BitmapResBase::`scalar deleting destructor'

// SIZE 0x24
// VTABLE: LEMBALL 0x00497918
class BitmapRes : public Primitive {
public:
	BitmapRes();
	virtual void Draw(Gdi* p_gdi);   // vtable+0x04
	virtual void Render(Gdi* p_gdi); // vtable+0x08
	virtual ~BitmapRes() {}          // vtable+0x00

	friend class BaseFrontendDrawer;
	friend class MainOptions1Drawer;
	friend class MainOptions2Drawer;
	friend class Surface;
	friend class CdLoadAnimDraw;
	friend class TargetAboutScreen;

private:
	short m_x;                // 0x04
	short m_y;                // 0x06
	short m_width;            // 0x08
	short m_height;           // 0x0a
	short m_sourceX;          // 0x0c
	short m_sourceY;          // 0x0e
	ResBitmap* m_resource;    // 0x10
	unsigned int m_flags;     // 0x14
	Remap* m_remap;           // 0x18
	unsigned int m_unknown1c; // 0x1c
	unsigned int m_unknown20; // 0x20
};

// SYNTHETIC: LEMBALL 0x00447170
// BitmapRes::`scalar deleting destructor'

#endif
