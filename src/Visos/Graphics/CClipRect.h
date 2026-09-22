#ifndef LEMBALL_VISOS_GRAPHICS_CCLIPRECT_H
#define LEMBALL_VISOS_GRAPHICS_CCLIPRECT_H

#include "CPrimitive.h" // complete type

// SIZE 0x10
// VTABLE: LEMBALL 0x00496cc8
class CClipRect : public CPrimitive {
public:
	CClipRect();
	virtual void Draw(CGDI* p_gdi);   // vtable+0x04
	virtual void Render(CGDI* p_gdi); // vtable+0x08
	// FUNCTION: LEMBALL 0x00432ac0
	virtual ~CClipRect() {} // vtable+0x00

	friend class CPVButton;
	friend class CTrackWindow;
	friend class CSurface;
	friend class CCDLoadAnim;
	friend class CFramedButton;

private:
	short m_left;              // 0x04
	short m_top;               // 0x06
	short m_right;             // 0x08
	short m_bottom;            // 0x0a
	unsigned int m_reserved0c; // 0x0c
};

// SYNTHETIC: LEMBALL 0x00432a60
// CClipRect::`scalar deleting destructor'

// SYNTHETIC: LEMBALL 0x00467bb0
// CClipRect::`vector deleting destructor'

#endif
