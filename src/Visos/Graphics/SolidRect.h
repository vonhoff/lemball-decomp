#ifndef LEMBALL_VISOS_GRAPHICS_SOLIDRECT_H
#define LEMBALL_VISOS_GRAPHICS_SOLIDRECT_H

#include "../../Common.h"
#include "Primitive.h" // complete type

// SIZE 0x10
// VTABLE: LEMBALL 0x00496cb8
class SolidRect : public Primitive {
public:
	SolidRect();
	virtual void Draw(Gdi* p_gdi);   // vtable+0x04
	virtual void Render(Gdi* p_gdi); // vtable+0x08
	virtual ~SolidRect();            // vtable+0x00

	friend class GunController;

private:
	short m_left;         // 0x04
	short m_top;          // 0x06
	short m_right;        // 0x08
	short m_bottom;       // 0x0a
	unsigned int m_color; // 0x0c
};

// SYNTHETIC: LEMBALL 0x00432a90
// SolidRect::`scalar deleting destructor'


#endif

