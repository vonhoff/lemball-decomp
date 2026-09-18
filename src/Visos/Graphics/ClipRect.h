#ifndef LEMBALL_VISOS_GRAPHICS_CLIPRECT_H
#define LEMBALL_VISOS_GRAPHICS_CLIPRECT_H

#include "Primitive.h" // complete type

// SIZE 0x10
// VTABLE: LEMBALL 0x00496cc8
class ClipRect : public Primitive {
public:
	ClipRect();
	virtual void Draw(Gdi* p_gdi);   // vtable+0x04
	virtual void Render(Gdi* p_gdi); // vtable+0x08
	// 68K 0x10101afa __dt__9CClipRectFv
	// FUNCTION: LEMBALL 0x00432ac0
	virtual ~ClipRect() {} // vtable+0x00

	friend class PvButton;
	friend class TrackWindow;
	friend class Surface;
	friend class CdLoadAnim;
	friend class TargetFramedButton;

private:
	short m_left;              // 0x04
	short m_top;               // 0x06
	short m_right;             // 0x08
	short m_bottom;            // 0x0a
	unsigned int m_reserved0c; // 0x0c
};

// SYNTHETIC: LEMBALL 0x00432a60
// ClipRect::`scalar deleting destructor'

// SYNTHETIC: LEMBALL 0x00467bb0
// ClipRect::`vector deleting destructor'

#endif
