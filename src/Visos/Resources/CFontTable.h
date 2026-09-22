#ifndef LEMBALL_VISOS_RESOURCES_CFONTTABLE_H
#define LEMBALL_VISOS_RESOURCES_CFONTTABLE_H

#include "CPvFontTable.h" // complete type

class CResFont;
class CResZrle;

// SIZE 0x08
// VTABLE: LEMBALL 0x0049a480
class CFontTable : public CPvFontTable {
public:
	CFontTable(CResFont* p_arg0);
	virtual CResZrle* GetZrle(int p_character); // vtable+0x04
	virtual char GetChar(CResZrle* p_glyph);    // vtable+0x08
	virtual ~CFontTable();                      // vtable+0x00

private:
	CResZrle** m_glyphs; // 0x04
};

// SYNTHETIC: LEMBALL 0x00473750
// CFontTable::`scalar deleting destructor'

#endif
