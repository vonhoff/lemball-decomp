#ifndef LEMBALL_VISOS_FOUNDATION_TEXT_H
#define LEMBALL_VISOS_FOUNDATION_TEXT_H

#include "../../Common.h"
#include "../Graphics/Zrle.h" // complete type
#include "String.h"           // complete type

// SIZE 0x44
// VTABLE: LEMBALL 0x00499ae8 primary
// Subobject VTABLE: 0x00499628 Zrle subobject at +0x2c
class Text : public Primitive {
public:
	inline Text()
		: m_x(0), m_y(0), m_offsetX(0), m_offsetY(0), m_startX(0), m_startY(0), m_advanceX(0), m_advanceY(0),
		  m_glyph()
	{
	}

	void NextPos();
	virtual ~Text();               // vtable+0x00
	virtual void Draw(Gdi* p_gdi);   // vtable+0x04
	virtual void Render(Gdi* p_gdi); // vtable+0x08
	virtual void Set(int p_x,
					 int p_y,
					 ResFont* p_font,
					 char* p_text,
					 unsigned long p_flags,
					 Remap* p_remap); // vtable+0x18
	virtual void Set(VsPoint& p_position,
					 ResFont* p_font,
					 char* p_text,
					 unsigned long p_flags,
					 Remap* p_remap); // vtable+0x14
	virtual void Set(int p_x,
					 int p_y,
					 ResFont* p_font,
					 String p_text,
					 unsigned long p_flags,
					 Remap* p_remap); // vtable+0x10
	virtual void Set(VsPoint& p_position,
					 ResFont* p_font,
					 String p_text,
					 unsigned long p_flags,
					 Remap* p_remap); // vtable+0x0c

	friend class TextManager;

protected:
	ResFont* m_font;           // 0x04
	const char* m_text;        // 0x08
	short m_x;                 // 0x0c
	short m_y;                 // 0x0e
	unsigned int m_flags;      // 0x10
	Remap* m_remap;            // 0x14
	short m_offsetX;           // 0x18
	short m_offsetY;           // 0x1a
	short m_startX;            // 0x1c
	short m_startY;            // 0x1e
	unsigned int m_useAdvance; // 0x20
	short m_advanceX;          // 0x24
	short m_advanceY;          // 0x26
	ResZrle* m_glyph;          // 0x28
	Zrle m_primitive;          // 0x2c
};

// SYNTHETIC: LEMBALL 0x00469b80
// Text::`scalar deleting destructor'


#endif

