#ifndef LEMBALL_VISOS_FOUNDATION_CTEXT_H
#define LEMBALL_VISOS_FOUNDATION_CTEXT_H

#include "../Graphics/CPrimitive.h"
#include "../Graphics/CZRLE.h" // complete type
#include "CString.h"           // complete type

class CRemap;
class CResFONT;
class CResZRLE;
struct CVsPoint;

// SIZE 0x44
// VTABLE: LEMBALL 0x00499ae8
// Subobject VTABLE: 0x00499628 CZRLE subobject at +0x2c
class CText : public CPrimitive {
public:
	inline CText() : m_x(m_y = 0), m_offsetX(m_offsetY = 0), m_startX(m_startY = 0), m_advanceX(m_advanceY = 0) {}

	void NextPos();
	virtual void Draw(CGDI* p_gdi);   // vtable+0x04
	virtual void Render(CGDI* p_gdi); // vtable+0x08
	virtual void Set(int p_x,
					 int p_y,
					 CResFONT* p_font,
					 char* p_text,
					 unsigned long p_flags,
					 CRemap* p_remap); // vtable+0x18
	virtual void Set(CVsPoint& p_position,
					 CResFONT* p_font,
					 char* p_text,
					 unsigned long p_flags,
					 CRemap* p_remap); // vtable+0x14
	virtual void Set(int p_x,
					 int p_y,
					 CResFONT* p_font,
					 CString p_text,
					 unsigned long p_flags,
					 CRemap* p_remap); // vtable+0x10
	virtual void Set(CVsPoint& p_position,
					 CResFONT* p_font,
					 CString p_text,
					 unsigned long p_flags,
					 CRemap* p_remap); // vtable+0x0c

	friend class CTextManager;

protected:
	CResFONT* m_font;          // 0x04
	const char* m_text;        // 0x08
	short m_x;                 // 0x0c
	short m_y;                 // 0x0e
	unsigned int m_flags;      // 0x10
	CRemap* m_remap;           // 0x14
	short m_offsetX;           // 0x18
	short m_offsetY;           // 0x1a
	short m_startX;            // 0x1c
	short m_startY;            // 0x1e
	unsigned int m_useAdvance; // 0x20
	short m_advanceX;          // 0x24
	short m_advanceY;          // 0x26
	CResZRLE* m_glyph;         // 0x28
	CZRLE m_primitive;         // 0x2c
};

// SYNTHETIC: LEMBALL 0x00469a00
// CText::CText

// SYNTHETIC: LEMBALL 0x00469b80
// CText::`vector deleting destructor'

// SYNTHETIC: LEMBALL 0x00469bf0
// CText::~CText

#endif
