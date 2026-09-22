#ifndef LEMBALL_VISOS_GRAPHICS_CSOLIDRECT_H
#define LEMBALL_VISOS_GRAPHICS_CSOLIDRECT_H

#include "../Foundation/CVsRect.h"
#include "CPrimitive.h" // complete type

// SIZE 0x10
// VTABLE: LEMBALL 0x00496cb8
class CSolidRect : public CPrimitive {
public:
	// FUNCTION: LEMBALL 0x00439520
	CSolidRect() {}
	CVsRect* GetBounds();
	virtual void Draw(CGDI* p_gdi);   // vtable+0x04
	virtual void Render(CGDI* p_gdi); // vtable+0x08
	// FUNCTION: LEMBALL 0x00439730 FOLDED
	virtual ~CSolidRect() {} // vtable+0x00

	friend class CGunController;
	friend class CAboutScreen;
	friend class CHiliteController;
	friend class CPasswordDrawer;
	friend class CSurface;
	friend class C2D;
	friend class CPVButton;

private:
	CVsRect m_bounds;     // 0x04
	unsigned int m_color; // 0x0c
};

// SYNTHETIC: LEMBALL 0x00432a90
// CSolidRect::`scalar deleting destructor'

#endif
