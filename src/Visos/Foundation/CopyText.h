#ifndef LEMBALL_SCAFFOLD_VISOS_FOUNDATION_COPYTEXT_H
#define LEMBALL_SCAFFOLD_VISOS_FOUNDATION_COPYTEXT_H

#include "../../Common.h"
#include "String.h" // complete type
#include "Text.h"   // complete type

// SIZE 0x4c
// VTABLE: LEMBALL 0x00499ce8
class CopyText : public Text {
public:
	virtual void Set(const VsPoint& p_position,
					 ResFont* p_font,
					 String p_text,
					 unsigned long p_flags,
					 Remap* p_remap); // vtable+0x0c
	virtual void Set(const VsPoint& p_position,
					 ResFont* p_font,
					 const char* p_text,
					 unsigned long p_flags,
					 Remap* p_remap); // vtable+0x14
	virtual void Set(int p_x,
					 int p_y,
					 ResFont* p_font,
					 String p_text,
					 unsigned long p_flags,
					 Remap* p_remap); // vtable+0x10
	virtual void Set(int p_x,
					 int p_y,
					 ResFont* p_font,
					 const char* p_text,
					 unsigned long p_flags,
					 Remap* p_remap); // vtable+0x18
	virtual ~CopyText();              // vtable+0x00

private:
	char* m_buffer;          // 0x44
	unsigned int m_capacity; // 0x48
};

#endif
