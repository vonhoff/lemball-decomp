#ifndef LEMBALL_VISOS_RESOURCES_CRESPALETTE_H
#define LEMBALL_VISOS_RESOURCES_CRESPALETTE_H

#include "CResBase.h" // complete type

// SIZE 0x04
struct PaletteHeader {
	unsigned int m_paletteState;
};

// SIZE 0x4c
// VTABLE: LEMBALL 0x00498c08
class CResPalette : public CResBase {
public:
	inline CResPalette(unsigned int p_resourceId) { DoLoad(p_resourceId); }
	static CResPalette* Load(unsigned int p_resourceId);
	virtual void SetHeader(); // vtable+0x08
	virtual void SetType();   // vtable+0x34

	friend void BuildSurfaceColourTable(unsigned int* p_entries,
										CResPalette* p_palette,
										void* p_unused,
										unsigned int* p_fallbackEntries);
	friend class C2D;
	friend class CBaseRemap;

private:
	unsigned int m_paletteState; // 0x48
};

// SYNTHETIC: LEMBALL 0x0045ea50
// CResPalette::`scalar deleting destructor'

#endif
