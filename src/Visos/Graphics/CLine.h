#ifndef LEMBALL_VISOS_GRAPHICS_CLINE_H
#define LEMBALL_VISOS_GRAPHICS_CLINE_H

#include "../Foundation/CVSPoint.h"
#include "CPrimitive.h"

// SIZE 0x10
// VTABLE: LEMBALL 0x00496cc8
class CLine : public CPrimitive {
public:
	CLine();
	virtual void Draw(CGDI* p_gdi);   // vtable+0x04
	virtual void Render(CGDI* p_gdi); // vtable+0x08
	// FUNCTION: LEMBALL 0x00432ac0
	virtual ~CLine() {} // vtable+0x00

	friend class CPVButton;
	friend class CTrackWindow;
	friend class CSurface;
	friend class CCDLoadAnim;
	friend class CFramedButton;

private:
	CVSPoint m_start;      // 0x04
	CVSPoint m_end;        // 0x08
	unsigned int m_colour; // 0x0c
};

// SYNTHETIC: LEMBALL 0x00432a60
// CLine::`scalar deleting destructor'

// SYNTHETIC: LEMBALL 0x00467bb0
// CLine::`vector deleting destructor'

#endif
