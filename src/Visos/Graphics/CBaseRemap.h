#ifndef LEMBALL_VISOS_GRAPHICS_CBASEREMAP_H
#define LEMBALL_VISOS_GRAPHICS_CBASEREMAP_H

enum ePaletteTypes {
	PALETTE_DEFAULT = 0,
	PALETTE_GREYSCALE = 1,
	PALETTE_MAPPED = 2,
};

class CResPalette;

// SIZE 0x08
class CBaseRemap {
public:
	CBaseRemap(unsigned long p_arg0, unsigned char* p_arg1, ePaletteTypes p_arg2);
	void CalculateGreyScale();
	void MapRemap(const unsigned char* p_mapping);
	~CBaseRemap();
	CBaseRemap();

	friend class CSurface;

public:
	unsigned char* m_remap;         // 0x00
	CResPalette* m_paletteResource; // 0x04
};

#endif
