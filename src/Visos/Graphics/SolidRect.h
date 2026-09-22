#ifndef LEMBALL_VISOS_GRAPHICS_SOLIDRECT_H
#define LEMBALL_VISOS_GRAPHICS_SOLIDRECT_H

#include "../Foundation/VsRect.h"
#include "Primitive.h" // complete type

// SIZE 0x10
// VTABLE: LEMBALL 0x00496cb8
class SolidRect : public Primitive {
public:
	// 68K 0x1010a6c2 __ct__10CSolidRectFv
	// FUNCTION: LEMBALL 0x00439520
	SolidRect() {}
	VsRect* GetBounds();
	virtual void Draw(Gdi* p_gdi);   // vtable+0x04
	virtual void Render(Gdi* p_gdi); // vtable+0x08
	// FUNCTION: LEMBALL 0x00439730 FOLDED
	virtual ~SolidRect() {} // vtable+0x00

	friend class GunController;
	friend class TargetAboutScreen;
	friend class HiliteController;
	friend class PasswordDrawer;
	friend class Surface;
	friend class C2D;
	friend class PvButton;

private:
	VsRect m_bounds;      // 0x04
	unsigned int m_color; // 0x0c
};

// SYNTHETIC: LEMBALL 0x00432a90
// SolidRect::`scalar deleting destructor'

#endif
