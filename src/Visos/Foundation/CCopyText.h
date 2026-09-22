#ifndef LEMBALL_VISOS_FOUNDATION_CCOPYTEXT_H
#define LEMBALL_VISOS_FOUNDATION_CCOPYTEXT_H

#include "CString.h" // complete type
#include "CText.h"   // complete type

// SIZE 0x4c
// VTABLE: LEMBALL 0x00499ce8
class CCopyText : public CText {
public:
	inline CCopyText(unsigned int p_maxLen)
	{
		m_capacity = p_maxLen;
		m_buffer = new char[p_maxLen + 1];
	}

	inline virtual ~CCopyText() { delete[] m_buffer; }
	virtual void Set(int p_x,
					 int p_y,
					 CResFont* p_font,
					 char* p_text,
					 unsigned long p_flags,
					 CRemap* p_remap); // vtable+0x18
	virtual void Set(CVsPoint& p_position,
					 CResFont* p_font,
					 char* p_text,
					 unsigned long p_flags,
					 CRemap* p_remap); // vtable+0x14
	virtual void Set(int p_x,
					 int p_y,
					 CResFont* p_font,
					 CString p_text,
					 unsigned long p_flags,
					 CRemap* p_remap); // vtable+0x10
	virtual void Set(CVsPoint& p_position,
					 CResFont* p_font,
					 CString p_text,
					 unsigned long p_flags,
					 CRemap* p_remap); // vtable+0x0c

	friend class CTextManager;

private:
	char* m_buffer;          // 0x44
	unsigned int m_capacity; // 0x48
};

// SYNTHETIC: LEMBALL 0x0046a220
// CCopyText::`scalar deleting destructor'

#endif
