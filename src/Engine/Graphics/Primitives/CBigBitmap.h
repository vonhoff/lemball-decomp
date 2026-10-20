#ifndef LEMBALL_VISOS_GRAPHICS_CBIGBITMAP_H
#define LEMBALL_VISOS_GRAPHICS_CBIGBITMAP_H

#include "CBitmap.h"

class CGDI;

// SIZE 0x24
// VTABLE: LEMBALL 0x00497918
class CBigBitmap : public CBitmap {
public:
	// FUNCTION: LEMBALL 0x00447120
	CBigBitmap()
	{
		m_unknown22 = 0;
		m_unknown20 = 0;
	}
	virtual void Draw(CGDI* p_gdi);   // vtable+0x04
	virtual void Render(CGDI* p_gdi); // vtable+0x08
	// FUNCTION: LEMBALL 0x00447260
	virtual ~CBigBitmap() {} // vtable+0x00

private:
	unsigned int m_unknown1c; // 0x1c
	short m_unknown20;        // 0x20
	short m_unknown22;        // 0x22
};

// SYNTHETIC: LEMBALL 0x00447170
// CBigBitmap::`scalar deleting destructor'

#endif
