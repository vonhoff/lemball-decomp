#ifndef LEMBALL_VISOS_RESOURCES_RESPALETTE_H
#define LEMBALL_VISOS_RESOURCES_RESPALETTE_H

#include "../../Common.h"
#include "ResBase.h" // complete type

// SIZE 0x04
struct PaletteHeader {
	undefined4 m_paletteState;
};

// SIZE 0x4c
// VTABLE: LEMBALL 0x00498c08
class ResPalette : public ResBase {
public:
	inline ResPalette(unsigned int p_resourceId) { DoLoad(p_resourceId); }
	static ResPalette* Load(unsigned int p_resourceId);
	virtual void SetHeader(); // vtable+0x08
	virtual void SetType();   // vtable+0x34
	virtual ~ResPalette();    // vtable+0x00

private:
	undefined4 m_paletteState; // 0x48
};

#endif
