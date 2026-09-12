#ifndef LEMBALL_VISOS_GRAPHICS_BASEREMAP_H
#define LEMBALL_VISOS_GRAPHICS_BASEREMAP_H

#include "../../Common.h"

class ResPalette;

// SIZE 0x08
class BaseRemap {
public:
	BaseRemap(unsigned long p_arg0, unsigned char* p_arg1, ePaletteTypes p_arg2);
	void CalculateGreyScale();
	void MapRemap(const unsigned char* p_mapping);
	~BaseRemap();
	BaseRemap();

	friend class Surface;

public:
	unsigned char* m_remap;        // 0x00
	ResPalette* m_paletteResource; // 0x04
};

#endif
