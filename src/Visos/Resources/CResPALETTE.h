#ifndef LEMBALL_VISOS_RESOURCES_CRESPALETTE_H
#define LEMBALL_VISOS_RESOURCES_CRESPALETTE_H

#include "CResBase.h" // complete type

// SIZE 0x04
struct PaletteHeader {
	unsigned int m_paletteState;
};

// SIZE 0x4c
// VTABLE: LEMBALL 0x00498c08
class CResPALETTE : public CResBase {
public:
	inline CResPALETTE(unsigned int p_resourceId) { DoLoad(p_resourceId); }
	static CResPALETTE* Load(unsigned int p_resourceId);
	virtual void SetHeader(); // vtable+0x08
	virtual void SetType();   // vtable+0x34

	friend void BuildSurfaceColourTable(unsigned int* p_entries,
										CResPALETTE* p_palette,
										void* p_unused,
										unsigned int* p_fallbackEntries);
	friend class C2D;
	friend class CBaseRemap;

private:
	unsigned int m_paletteState; // 0x48
};

// SYNTHETIC: LEMBALL 0x0045ea50
// CResPALETTE::`scalar deleting destructor'

#endif
