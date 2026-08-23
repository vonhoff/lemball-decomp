#ifndef LEMBALL_VISOS_RESOURCES_FONTTABLE_H
#define LEMBALL_VISOS_RESOURCES_FONTTABLE_H

#include "../../Common.h"
#include "PvFontTable.h" // complete type

// SIZE 0x08
// VTABLE: LEMBALL 0x0049a480
class FontTable : public PvFontTable {
public:
	FontTable(ResFont* p_arg0);
	virtual ResZrle* GetZrle(int p_character); // vtable+0x04
	virtual char GetChar(ResZrle* p_glyph);    // vtable+0x08
	virtual ~FontTable();                      // vtable+0x00

private:
	ResZrle** m_glyphs; // 0x04
};

#endif
